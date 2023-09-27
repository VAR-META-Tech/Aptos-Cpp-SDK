//
// Created by Anh NPH on 25/09/2023.
//

#ifndef APTOS_PUBLICKEY_H
#define APTOS_PUBLICKEY_H
#include <string>
#include <vector>
#include <stdexcept>
#include "Signature.h"

class PublicKey
{
public:
    static const int KeyLength = 32;

private:
    std::string _key;
    std::vector<CryptoPP::byte> _keyBytes;

public:
    PublicKey(const std::vector<CryptoPP::byte>& publicKey);
    PublicKey(const std::string& key);

    std::string Key();
    void Key(const std::string& key);

    std::vector<CryptoPP::byte> KeyBytes();
    void KeyBytes(const std::vector<CryptoPP::byte>& bytes);

    bool Verify(const std::vector<CryptoPP::byte>& message, const Signature& signature);
    bool IsOnCurve() const;

    void Serialize(Serialization& serializer) const;
    static PublicKey Deserialize(Deserialization& deserializer);

    bool isEqual(PublicKey& other);
    bool isNotEqual(PublicKey& other);

    std::string ToString();
    size_t GetHashCode() const;
};


#endif //APTOS_PUBLICKEY_H
