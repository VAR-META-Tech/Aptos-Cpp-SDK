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

    std::string Key();
    void Key(std::string key);

    CryptoPP::SecByteBlock KeyBytes();
    void KeyBytes(CryptoPP::SecByteBlock value);

    PrivateKey(CryptoPP::SecByteBlock privateKey);
    PrivateKey(std::string key);
    PrivateKey(std::array<CryptoPP::byte, KeyLength> privateKey);

    static PrivateKey FromHex(std::string key);
    PublicKey GetPublicKey();
    static PrivateKey Random();

    bool operator ==(const PrivateKey& rhs) const;
    bool operator !=(const PrivateKey& rhs) const;

    Signature Sign(CryptoPP::SecByteBlock message);

    void Serialize(Serialization& serializer);

    bool Equals(const PrivateKey& rhs) const;
    int GetHashCode() const;
    std::string ToString();

private:
    std::string _key;
    CryptoPP::SecByteBlock _extendedKeyBytes;
    CryptoPP::SecByteBlock _keyBytes;

    void GenerateExtendedKey();


};



#endif //APTOS_PRIVATEKEY_H
