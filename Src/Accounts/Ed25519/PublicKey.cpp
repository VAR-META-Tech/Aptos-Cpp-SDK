//
// Created by Anh NPH on 25/09/2023.
//

#include "PublicKey.h"
#include "../Ed25519Signature.h"
#include "../../HDWallet/Utils/Utils.h"
#include "cryptopp/hex.h"
#include "cryptopp/xed25519.h"

namespace Aptos::Accounts::Ed25519
{
    PublicKey::PublicKey(const CryptoPP::SecByteBlock &publicKey)
    {
        if (publicKey.empty())
        {
            throw std::invalid_argument("PublicKey cannot be null.");
        }
        if (publicKey.size() != KeyLength)
        {
            throw std::invalid_argument("Invalid key length.");
        }
        _keyBytes = publicKey;
    }

    PublicKey::PublicKey(std::string key)
    {
        if (!Utils::IsValidAddress(key))
        {
            throw std::invalid_argument("Invalid key.");
        }
        if (key.empty())
        {
            throw std::invalid_argument("Key cannot be null.");
        }
        if (key.starts_with("0x"))
        {
            key = key.substr(2);
        }
        CryptoPP::HexDecoder decoder;
        decoder.Put((CryptoPP::byte *)key.data(), key.size());
        decoder.MessageEnd();
        size_t size = decoder.MaxRetrievable();
        if (size && size <= SIZE_MAX)
        {
            _keyBytes.resize(size);
            decoder.Get((CryptoPP::byte *)&_keyBytes[0], _keyBytes.size());
        }
    }

    std::string PublicKey::Key() const
    {
        std::string key = "0x";
        CryptoPP::StringSource ss(_keyBytes.data(), _keyBytes.size(), true,
                                  new CryptoPP::HexEncoder(
                                      new CryptoPP::StringSink(key),
                                      false) // HexDecoder
        );                                   // StringSource
        return key;
    }

    void PublicKey::setKey(std::string key)
    {
        if (!Utils::IsValidAddress(key))
        {
            throw std::invalid_argument("Invalid key.");
        }
        if (key.empty())
        {
            throw std::invalid_argument("Key cannot be null.");
        }
        CryptoPP::HexDecoder decoder;
        decoder.Put((CryptoPP::byte *)key.data(), key.size());
        decoder.MessageEnd();
        size_t size = decoder.MaxRetrievable();
        if (size && size <= SIZE_MAX)
        {
            _keyBytes.resize(size);
            decoder.Get((CryptoPP::byte *)&_keyBytes[0], _keyBytes.size());
        }
    }

    CryptoPP::SecByteBlock PublicKey::KeyBytes() const
    {
        return _keyBytes;
    }

    void PublicKey::setKeyBytes(const CryptoPP::SecByteBlock &bytes)
    {
        if (bytes.empty())
        {
            throw std::invalid_argument("Key bytes cannot be null.");
        }
        if (bytes.size() != KeyLength)
        {
            throw std::invalid_argument("Invalid key length.");
        }
        _keyBytes = bytes;
    }

    bool PublicKey::Verify(const CryptoPP::SecByteBlock &message, const Ed25519Signature &signature) const
    {
        CryptoPP::ed25519::Verifier verifier(_keyBytes);
        return verifier.VerifyMessage(message.data(), message.size(),
                                      signature.Data().data(), signature.Data().size());
    }

    bool PublicKey::IsOnCurve() const
    {
        return false;
    }

    void PublicKey::Serialize(BCS::Serialization &serializer) const
    {
        std::vector<uint8_t> byteArray = Utils::SecBlockToByteVector(_keyBytes);
        serializer.SerializeBytes(byteArray);
    }

    std::shared_ptr<BCS::ISerializable> PublicKey::Deserialize(BCS::Deserialization &deserializer)
    {
        auto bytes = deserializer.ToBytes();
        CryptoPP::SecByteBlock keyBytes = Utils::ByteVectorToSecBlock(bytes);
        if (keyBytes.size() != KeyLength)
        {
            throw std::runtime_error("Length mismatch. Expected: " + std::to_string(KeyLength) + ", Actual: " + std::to_string(keyBytes.size()));
        }
        return std::make_shared<PublicKey>(keyBytes);
    }

    std::string PublicKey::ToString() const
    {
        return Key();
    }

    size_t PublicKey::GetHashCode() const
    {
        return std::hash<std::string>{}(Key());
    }

    bool PublicKey::Equals(const PublicKey &rhs) const
    {
        return Key() == rhs.Key();
    }

    bool PublicKey::operator==(const PublicKey &rhs) const
    {
        return Equals(rhs);
    }
}