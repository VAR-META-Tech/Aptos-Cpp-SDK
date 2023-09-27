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
#include "../HDWallet/Utils/Utils.h"

std::string PrivateKey::Key() {
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

void PrivateKey::Key(std::string key) {
    _key = key;
}

std::vector<CryptoPP::byte> PrivateKey::KeyBytes() {
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
                GenerateExtendedKey();
            }
    }
    return _keyBytes;
}

void PrivateKey::KeyBytes(std::vector<CryptoPP::byte> value) {
    if (value.empty())
        throw std::invalid_argument("PublicKey cannot be null.");
    if (value.size() != KeyLength)
        throw std::invalid_argument("Invalid key length.");
    _keyBytes = value;
}

PrivateKey::PrivateKey(std::string key) {
    if (!Utils::IsValidAddress(key))
        throw std::invalid_argument("Invalid key.");
    if (key.empty())
        throw std::invalid_argument("Key cannot be null.");
    _key = key;
}

PrivateKey::PrivateKey(std::array<CryptoPP::byte, KeyLength> privateKey) {
//todo
}

PrivateKey PrivateKey::FromHex(std::string key) {
    return PrivateKey(key);
}

PublicKey PrivateKey::GetPublicKey() {
    if (_keyBytes.empty()){
        KeyBytes();
    }
    std::vector<CryptoPP::byte> publicKeyBytes;
    publicKeyBytes.resize(CryptoPP::ed25519PublicKey::PUBLIC_KEYLENGTH);
    this->SecretToPublicKey(publicKeyBytes.data(),_keyBytes.data());
    return PublicKey(publicKeyBytes);
}

bool PrivateKey::operator!=(const PrivateKey &rhs) const {
    return Equals(rhs);
}

Signature PrivateKey::Sign(std::vector<CryptoPP::byte> message) {
    //CryptoPP::AutoSeededRandomPool prng;
    if (_keyBytes.empty()){
        KeyBytes();
    }
    CryptoPP::ed25519::Signer signer(_keyBytes.data());
    std::string signature;
    size_t siglen = signer.MaxSignatureLength();
    signature.resize(siglen);
    signer.SignMessage(CryptoPP::NullRNG(), message.data(), message.size(),
                       (CryptoPP::byte*)&signature[0]);
    signature.resize(siglen);
    std::vector<CryptoPP::byte> signatureData;
    signatureData.resize(signature.size());
    std::copy(signature.begin(), signature.end(), signatureData.begin());
    return Signature(signatureData);
}

void PrivateKey::Serialize(Serialization serializer) {

}

bool PrivateKey::Equals(const PrivateKey &rhs) const {
    return _key == rhs._key;
}

int PrivateKey::GetHashCode() const {
    return 0;
}

std::string PrivateKey::ToString(){
    return Key();
}

PrivateKey PrivateKey::Random() {
    std::vector<CryptoPP::byte> seed(KeyLength);
    CryptoPP::AutoSeededRandomPool rng;
    rng.GenerateBlock(seed.data(), seed.size());
    return PrivateKey(seed);
}

PrivateKey::PrivateKey(std::vector<CryptoPP::byte> privateKey) {
    if (privateKey.empty())
        throw std::invalid_argument("PublicKey cannot be null.");
    if (privateKey.size() != KeyLength)
        throw std::invalid_argument("Invalid key length.");
    _keyBytes = privateKey;
}

bool PrivateKey::operator==(const PrivateKey &rhs) const {
    return Equals(rhs);
}

void PrivateKey::GenerateExtendedKey() {

    CryptoPP::SecByteBlock extendedKey(ExtendedKeyLength);

    CryptoPP::SHA512 sha;
    sha.CalculateDigest(extendedKey,
                        _keyBytes.data(), _keyBytes.size());

    _extendedKeyBytes.assign(extendedKey.begin(), extendedKey.end());
}
