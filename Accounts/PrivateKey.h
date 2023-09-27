//
// Created by Anh NPH on 26/09/2023.
//

#ifndef APTOS_PRIVATEKEY_H
#define APTOS_PRIVATEKEY_H

#include <string>
#include <vector>
#include "Signature.h"
#include "PublicKey.h"
#include "../BCS/Serialization.h"
#include <stdexcept>
#include <cryptopp/xed25519.h>

class PrivateKey : public CryptoPP::ed25519PrivateKey
{
public:
    static const int ExtendedKeyLength = 64;
    static const int KeyLength = 32;

private:
    std::string _key;
    std::vector<CryptoPP::byte> _extendedKeyBytes;
    std::vector<CryptoPP::byte> _keyBytes;

    void GenerateExtendedKey();

public:
    std::string Key();
    void Key(std::string key);

    std::vector<CryptoPP::byte> KeyBytes();
    void KeyBytes(std::vector<CryptoPP::byte> value);

    PrivateKey(std::vector<CryptoPP::byte> privateKey);
    PrivateKey(std::string key);
    PrivateKey(std::array<CryptoPP::byte, KeyLength> privateKey);

    static PrivateKey FromHex(std::string key);
    PublicKey GetPublicKey();
    static PrivateKey Random();

    bool operator ==(const PrivateKey& rhs) const;
    bool operator !=(const PrivateKey& rhs) const;

    Signature Sign(std::vector<CryptoPP::byte> message);

    void Serialize(Serialization serializer);

    bool Equals(const PrivateKey& rhs) const;
    int GetHashCode() const;
    std::string ToString();
};



#endif //APTOS_PRIVATEKEY_H
