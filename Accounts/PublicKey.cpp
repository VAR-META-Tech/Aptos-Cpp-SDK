//
// Created by Anh NPH on 25/09/2023.
//

#include "PublicKey.h"
#include "Signature.h"
#include "../HDWallet/Utils/Utils.h"
#include "cryptopp/hex.h"
#include "cryptopp/xed25519.h"

PublicKey::PublicKey(const std::vector<CryptoPP::byte>& publicKey)
{
    if (publicKey.empty())
        throw std::invalid_argument("PublicKey cannot be null.");
    if (publicKey.size() != KeyLength)
        throw std::invalid_argument("Invalid key length.");
    _keyBytes = publicKey;
}

PublicKey::PublicKey(const std::string& key)
{
    if (!Utils::IsValidAddress(key))
        throw std::invalid_argument("Invalid key.");
    if (key.empty())
        throw std::invalid_argument("Key cannot be null.");
    _key = key;
}

std::string PublicKey::Key()
{
    if(_key.empty() && !_keyBytes.empty())
    {
        _key = "0x";
        CryptoPP::StringSource ss(_keyBytes.data(), _keyBytes.size(), true,
                                  new CryptoPP::HexEncoder(
                                new CryptoPP::StringSink(_key),
                                false
                        ) // HexDecoder
        ); // StringSource
    }
    return _key;
}

void PublicKey::Key(const std::string& key)
{
    if (!Utils::IsValidAddress(key))
        throw std::invalid_argument("Invalid key.");
    if (key.empty())
        throw std::invalid_argument("Key cannot be null.");
    _key = key;
}

std::vector<CryptoPP::byte> PublicKey::KeyBytes()
{
    if (_keyBytes.empty() && !_key.empty())
    {
        std::string key = _key;
        if (key.substr(0, 2) == "0x") {
            key = key.substr(2);
        }
        CryptoPP::HexDecoder decoder;
        decoder.Put((CryptoPP::byte*)key.data(),key.size());
        decoder.MessageEnd();
        size_t size = decoder.MaxRetrievable();
        if(size && size <= SIZE_MAX)
        {
            _keyBytes.resize(size);
            decoder.Get((CryptoPP::byte*)&_keyBytes[0], _keyBytes.size());
        }
    }
    return _keyBytes;
}

void PublicKey::KeyBytes(const std::vector<CryptoPP::byte>& bytes)
{
    if (bytes.empty())
        throw std::invalid_argument("Key bytes cannot be null.");
    if (bytes.size() != KeyLength)
        throw std::invalid_argument("Invalid key length.");
    _keyBytes = bytes;
}

bool PublicKey::Verify(const std::vector<CryptoPP::byte>& message, const Signature& signature) {
    CryptoPP::ed25519::Verifier verifier;
    verifier.VerifyMessage(message.data(), message.size(),
                           signature.Data().data(), signature.Data().size());
   return false;
}

bool PublicKey::IsOnCurve() const
{
   //todo
   return false;
}

void PublicKey::Serialize(Serialization& serializer)
{
    if (_keyBytes.empty()){
        KeyBytes();
    }
    serializer.SerializeBytes(this->_keyBytes);
}

PublicKey PublicKey::Deserialize(Deserialization& deserializer)
{
    std::vector<CryptoPP::byte> keyBytes = deserializer.ToBytes();
    if (keyBytes.size() != KeyLength)
        throw std::runtime_error("Length mismatch. Expected: " + std::to_string(KeyLength) + ", Actual: " + std::to_string(keyBytes.size()));
    return PublicKey(keyBytes);
}

std::string PublicKey::ToString()
{
    return Key();
}

size_t PublicKey::GetHashCode() const
{
    return std::hash<std::string>{}(_key);
}

bool PublicKey::Equals(const PublicKey &rhs) const {
    return _key == rhs._key;
}

bool PublicKey::operator==(const PublicKey &rhs) const {
    return Equals(rhs);
}

bool PublicKey::operator!=(const PublicKey &rhs) const {
    return !Equals(rhs);
}
