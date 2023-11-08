//
// Created by Anh NPH on 11/09/2023.
//

#ifndef APTOS_SIGNATURE_H
#define APTOS_SIGNATURE_H

#include <vector>
#include <string>
#include "../BCS/Serialization.h"
#include "../BCS/Deserialization.h"

class Signature: public ISerializable {
public:
    static const int SignatureLength = 64;

private:
    CryptoPP::SecByteBlock _signatureBytes;
    mutable std::string _signature;

public:
    Signature(const CryptoPP::SecByteBlock& signature);
    CryptoPP::SecByteBlock Data() const;
    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    bool operator==(const Signature& other) const;
    bool operator!=(const Signature& other) const;
    std::string ToString() const;
    bool Equals(const Signature& rhs) const;
};

#endif //APTOS_SIGNATURE_H
