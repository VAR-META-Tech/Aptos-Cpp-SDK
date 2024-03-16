//
// Created by Anh NPH on 26/09/2023.
//

#include "PrivateKey.h"
#include "cryptopp/hex.h"
#include "cryptopp/sha.h"
#include "cryptopp/secblock.h"
#include "cryptopp/xed25519.h"
#include "cryptopp/osrng.h"
#include <iostream>
#include "../../HDWallet/Utils/Utils.h"

namespace Aptos::Accounts::Ed25519
{
    std::string PrivateKey::Key()
    {
        if (_key.empty() && !_keyBytes.empty())
        {
            _key = "0x";
            CryptoPP::StringSource ss(_keyBytes.data(), _keyBytes.size(), true,
                                      new CryptoPP::HexEncoder(
                                          new CryptoPP::StringSink(_key),
                                          false) // HexDecoder
            );                                   // StringSource
        }
        return _key;
    }

    void PrivateKey::Key(std::string const &key)
    {
        _key = key;
    }

    CryptoPP::SecByteBlock PrivateKey::KeyBytes()
    {
        if (_keyBytes.empty() && !_key.empty())
        {
            std::string key = _key;
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
                GenerateExtendedKey();
            }
        }
        return _keyBytes;
    }

    void PrivateKey::KeyBytes(CryptoPP::SecByteBlock value)
    {
        if (value.empty())
            throw std::invalid_argument("PublicKey cannot be null.");
        if (value.size() != KeyLength)
            throw std::invalid_argument("Invalid key length.");
        _keyBytes = value;
    }

    PrivateKey::PrivateKey(std::string const &key)
    {
        if (key.empty())
            throw std::invalid_argument("Key cannot be null.");
        if (!Utils::IsValidAddress(key))
            throw std::invalid_argument("Invalid key.");
        _key = key;
    }

    PrivateKey PrivateKey::FromHex(std::string key)
    {
        return PrivateKey(key);
    }

    PublicKey PrivateKey::GetPublicKey()
    {
        if (_keyBytes.empty())
        {
            KeyBytes();
        }
        CryptoPP::SecByteBlock publicKeyBytes;
        publicKeyBytes.resize(CryptoPP::ed25519PublicKey::PUBLIC_KEYLENGTH);
        this->SecretToPublicKey(publicKeyBytes.data(), _keyBytes.data());
        return PublicKey(publicKeyBytes);
    }

    Ed25519Signature PrivateKey::Sign(CryptoPP::SecByteBlock message)
    {
        if (_keyBytes.empty())
        {
            KeyBytes();
        }
        CryptoPP::ed25519::Signer signer(_keyBytes.data());
        std::string signature;
        size_t siglen = signer.MaxSignatureLength();
        signature.resize(siglen);
        signer.SignMessage(CryptoPP::NullRNG(), message.data(), message.size(),
                           (CryptoPP::byte *)&signature[0]);
        signature.resize(siglen);
        CryptoPP::SecByteBlock signatureData;
        signatureData.resize(signature.size());
        std::ranges::copy(signature.begin(), signature.end(), signatureData.begin());
        return Ed25519Signature(signatureData);
    }

    void PrivateKey::Serialize(BCS::Serialization &serializer)
    {
        if (_keyBytes.empty())
        {
            KeyBytes();
        }
        auto bytes = Utils::SecBlockToByteVector(_keyBytes);
        serializer.SerializeBytes(bytes);
    }

    bool PrivateKey::Equals(const PrivateKey &rhs) const
    {
        return _key == rhs._key;
    }

    int PrivateKey::GetHashCode() const
    {
        return std::hash<std::string>{}(_key);
    }

    std::string PrivateKey::ToString()
    {
        return Key();
    }

    PrivateKey PrivateKey::Random()
    {
        CryptoPP::SecByteBlock seed(KeyLength);
        CryptoPP::AutoSeededRandomPool rng;
        rng.GenerateBlock(seed.data(), seed.size());
        return PrivateKey(seed);
    }

    PrivateKey::PrivateKey(CryptoPP::SecByteBlock privateKey)
    {
        if (privateKey.empty())
            throw std::invalid_argument("PublicKey cannot be null.");
        if (privateKey.size() != KeyLength)
            throw std::invalid_argument("Invalid key length.");
        _keyBytes = privateKey;
    }

    bool PrivateKey::operator==(const PrivateKey &rhs) const
    {
        return Equals(rhs);
    }

    void PrivateKey::GenerateExtendedKey()
    {

        CryptoPP::SecByteBlock extendedKey(ExtendedKeyLength);

        CryptoPP::SHA512 sha;
        sha.CalculateDigest(extendedKey,
                            _keyBytes.data(), _keyBytes.size());

        _extendedKeyBytes.Assign(extendedKey.data(), extendedKey.size());
    }
}