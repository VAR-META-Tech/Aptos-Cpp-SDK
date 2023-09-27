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

PrivateKey::PrivateKey(const std::string& hexKey) {
    // Parse hex string into bytes
    CryptoPP::StringSource(hexKey.substr(2), true,
                                        new CryptoPP::HexDecoder).Ref().Get(key_bytes_[0]);

    GenerateExtendedKey();
}

PrivateKey::PrivateKey(const std::vector<uint8_t>& seed) {
    if (seed.size() != KeyLength) {
        throw std::invalid_argument("Invalid seed size");
    }

    key_bytes_ = seed;

    GenerateExtendedKey();
}

void PrivateKey::GenerateExtendedKey() {
    // Generate extended key from seed
    CryptoPP::SecByteBlock extendedKey(ExtendedKeyLength);
    CryptoPP::SHA512().CalculateDigest(extendedKey, key_bytes_.data(), key_bytes_.size());

    extended_key_bytes_.assign(extendedKey.begin(), extendedKey.end());
}

std::vector<uint8_t> PrivateKey::Sign(const std::vector<uint8_t>& message) const {
    // Sign using CryptoPP
    CryptoPP::ed25519::Signer signer(extended_key_bytes_[0]);
    CryptoPP::SecByteBlock signature(64);
    signer.SignMessage(CryptoPP::NullRNG(), message.data(), message.size(),
                       signature);

    return {signature.begin(), signature.end()};
}

PublicKey PrivateKey::GetPublicKey() const {
    //todo need update logic
    std::vector<uint8_t> publicKeyBytes(PublicKey::KeyLength);
    CryptoPP::ed25519::Signer signer(extended_key_bytes_[0]);
    return PublicKey(publicKeyBytes);
}

bool PrivateKey::operator==(const PrivateKey& rhs) const {
    return key_bytes_ == rhs.key_bytes_;
}

PrivateKey PrivateKey::Random() {
    std::vector<uint8_t> seed(KeyLength);
    CryptoPP::AutoSeededRandomPool rng;
    rng.GenerateBlock(seed.data(), seed.size());
    return PrivateKey(seed);
}
