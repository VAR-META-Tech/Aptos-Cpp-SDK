//
// Created by Anh NPH on 11/09/2023.
//

#ifndef APTOS_SIGNATURE_H
#define APTOS_SIGNATURE_H

#include <vector>
#include <string>
#include "../BCS/Serialization.h"
#include "../BCS/Deserialization.h"

class Signature: public ISerializable{
public:
    static const int SignatureLength = 64;

private:
    std::vector<CryptoPP::byte> _signatureBytes;
    mutable std::string _signature;

public:
    Signature(const std::vector<CryptoPP::byte>& signature);
    std::vector<CryptoPP::byte> Data() const;
    void Serialize(Serialization& serializer) override;
    static Signature Deserialize(Deserialization& deserializer);
    bool operator==(const Signature& other) const;
    bool operator!=(const Signature& other) const;
    std::string ToString() const;
};

#endif //APTOS_SIGNATURE_H
