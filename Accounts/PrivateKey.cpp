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
        CryptoPP::StringSource(key, true, new CryptoPP::HexDecoder())
                .Ref().Get(_keyBytes[0]);
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

PublicKey PrivateKey::PublicKey() {
    return PublicKey();
}

bool PrivateKey::operator!=(const PrivateKey &rhs) const {
    return Equals(rhs);
}

Signature PrivateKey::Sign(std::vector<CryptoPP::byte> message) {
    CryptoPP::ed25519::Signer signer(_extendedKeyBytes.data());
    CryptoPP::SecByteBlock signature(64);
    signer.SignMessage(CryptoPP::NullRNG(), message.data(), message.size(),
                       signature);
    std::vector<CryptoPP::byte> signatureData(*signature.BytePtr(),64);
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
