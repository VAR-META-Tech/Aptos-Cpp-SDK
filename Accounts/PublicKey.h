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
    std::string key_;
    std::vector<uint8_t> keyBytes_;

public:
    PublicKey(const std::vector<uint8_t>& publicKey);
    PublicKey(const std::string& key);

    std::string getKey();
    void setKey(const std::string& key);

    std::vector<uint8_t> getKeyBytes();
    void setKeyBytes(const std::vector<uint8_t>& bytes);

    bool Verify(const std::vector<uint8_t>& message, const Signature& signature);
    bool IsOnCurve() const;

    void Serialize(Serialization& serializer) const;
    static PublicKey Deserialize(Deserialization& deserializer);

    bool isEqual(PublicKey& other);
    bool isNotEqual(PublicKey& other);

    std::string ToString();
    size_t GetHashCode() const;
};


#endif //APTOS_PUBLICKEY_H
