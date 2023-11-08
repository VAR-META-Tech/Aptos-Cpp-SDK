//
// Created by Anh NPH on 25/09/2023.
//

#ifndef APTOS_PUBLICKEY_H
#define APTOS_PUBLICKEY_H
#include <string>
#include <vector>
#include <stdexcept>
#include "Signature.h"

class PublicKey : public ISerializable
{
public:
    static const int KeyLength = 32;

private:
    CryptoPP::SecByteBlock _keyBytes;

public:
    PublicKey(const CryptoPP::SecByteBlock& publicKey);
    PublicKey(std::string key);

    std::string Key() const;
    void setKey(std::string key);

    CryptoPP::SecByteBlock KeyBytes() const;
    void setKeyBytes(const CryptoPP::SecByteBlock& bytes);

    bool Verify(const CryptoPP::SecByteBlock& message, const Signature& signature) const;
    bool IsOnCurve() const;

    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);

    std::string ToString() const;
    size_t GetHashCode() const;

    bool Equals(const PublicKey& rhs) const;

    bool operator ==(const PublicKey& rhs) const;
    bool operator !=(const PublicKey& rhs) const;
};


#endif //APTOS_PUBLICKEY_H
