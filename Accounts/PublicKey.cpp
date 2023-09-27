//
// Created by Anh NPH on 25/09/2023.
//

#include "PublicKey.h"
#include "Signature.h"
#include "../HDWallet/Utils/Utils.h"
#include "cryptopp/hex.h"

PublicKey::PublicKey(const std::vector<uint8_t>& publicKey)
{
    if (publicKey.empty())
        throw std::invalid_argument("PublicKey cannot be null.");
    if (publicKey.size() != KeyLength)
        throw std::invalid_argument("Invalid key length.");
    keyBytes_ = publicKey;
}

PublicKey::PublicKey(const std::string& key)
{
    if (!Utils::IsValidAddress(key))
        throw std::invalid_argument("Invalid key.");
    if (key.empty())
        throw std::invalid_argument("Key cannot be null.");
    key_ = key;
}

std::string PublicKey::getKey()
{
    if(key_.empty() && !keyBytes_.empty())
    {
        key_ = "0x";
        CryptoPP::StringSource ss(reinterpret_cast<const CryptoPP::byte *>(keyBytes_[0]), sizeof(keyBytes_), true,
                                  new CryptoPP::HexEncoder(
                                new CryptoPP::StringSink(key_),
                                true,   // uppercase
                                2,      // grouping
                                ""  // separator
                        ) // HexDecoder
        ); // StringSource
    }
    return key_;
}

void PublicKey::setKey(const std::string& key)
{
    if (!Utils::IsValidAddress(key))
        throw std::invalid_argument("Invalid key.");
    if (key.empty())
        throw std::invalid_argument("Key cannot be null.");
    key_ = key;
}

std::vector<uint8_t> PublicKey::getKeyBytes()
{
    if (keyBytes_.empty() && !key_.empty())
    {
        std::string key = key_;
        if (key.substr(0, 2) == "0x") {
            key = key.substr(2);
        }
        CryptoPP::StringSource(key, true, new CryptoPP::HexDecoder())
                .Ref().Get(keyBytes_[0]);
    }
    return keyBytes_;
}

void PublicKey::setKeyBytes(const std::vector<uint8_t>& bytes)
{
    if (bytes.empty())
        throw std::invalid_argument("Key bytes cannot be null.");
    if (bytes.size() != KeyLength)
        throw std::invalid_argument("Invalid key length.");
    keyBytes_ = bytes;
}

bool PublicKey::Verify(const std::vector<uint8_t>& message, const Signature& signature) {
   //todo
   return false;
}

bool PublicKey::IsOnCurve() const
{
   //todo
   return false;
}

void PublicKey::Serialize(Serialization& serializer) const
{
    serializer.SerializeBytes(this->keyBytes_);
}

PublicKey PublicKey::Deserialize(Deserialization& deserializer)
{
    std::vector<uint8_t> keyBytes = deserializer.ToBytes();
    if (keyBytes.size() != KeyLength)
        throw std::runtime_error("Length mismatch. Expected: " + std::to_string(KeyLength) + ", Actual: " + std::to_string(keyBytes.size()));
    return PublicKey(keyBytes);
}

bool PublicKey::isEqual(PublicKey& other)
{
    if (&other == this) return true;
    return other.getKey() == this->getKey();
}

bool PublicKey::isNotEqual(PublicKey& other)
{
    return !isEqual(other);
}

std::string PublicKey::ToString()
{
    return getKey();
}

size_t PublicKey::GetHashCode() const
{
    return std::hash<std::string>{}(key_);
}