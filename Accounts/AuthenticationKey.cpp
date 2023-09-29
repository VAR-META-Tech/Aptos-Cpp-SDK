//
// Created by Anh NPH on 28/09/2023.
//

#include "AuthenticationKey.h"
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

AuthenticationKey::AuthenticationKey(const std::vector<CryptoPP::byte> &bytes) {
    if (bytes.size() != LENGTH)
        throw std::invalid_argument("Byte array must be " + std::to_string(LENGTH) + " bytes");
    _bytes = bytes;
}

AuthenticationKey
AuthenticationKey::FromMultiEd25519PublicKey(Aptos::Accounts::Types::MultiEd25519PublicKey publicKey) {
    CryptoPP::SHA256 hash;
    std::vector<CryptoPP::byte> pubKeyBytes = publicKey.ToBytes();
    std::vector<CryptoPP::byte> bytes;
    bytes.resize(pubKeyBytes.size()+1);
    std::copy(pubKeyBytes.begin(),pubKeyBytes.end(),bytes.begin());
    bytes[pubKeyBytes.size()] = AuthenticationKey::MULTI_ED25519_SCHEME;
    hash.Update(bytes.data(), bytes.size());
    std::vector<CryptoPP::byte> result;
    result.resize(CryptoPP::ed25519PublicKey::PUBLIC_KEYLENGTH);
    hash.Final(result.data());
    return AuthenticationKey(result);
}

AuthenticationKey AuthenticationKey::FromEd25519PublicKey(const std::vector<CryptoPP::byte> &publicKey) {
    CryptoPP::SHA256 hash;
    std::vector<CryptoPP::byte> bytes;
    bytes.resize(publicKey.size()+1);
    std::copy(publicKey.begin(),publicKey.end(),bytes.begin());
    bytes[publicKey.size()] = AuthenticationKey::MULTI_ED25519_SCHEME;
    hash.Update(bytes.data(), bytes.size());
    std::vector<CryptoPP::byte> result;
    result.resize(CryptoPP::ed25519PublicKey::PUBLIC_KEYLENGTH);
    hash.Final(result.data());
    return AuthenticationKey(result);
}

std::string AuthenticationKey::DerivedAddress() {
    std::string hexString;
    CryptoPP::StringSource ss(_bytes.data(), _bytes.size(), true,
                              new CryptoPP::HexEncoder(
                                      new CryptoPP::StringSink(hexString),
                                      false
                              ) // HexDecoder
    );
    return hexString;
}