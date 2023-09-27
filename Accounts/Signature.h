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
    std::vector<uint8_t> _signatureBytes;
    mutable std::string _signature;

public:
    Signature(const std::vector<uint8_t>& signature);
    std::vector<uint8_t> Data() const;
    void Serialize(Serialization& serializer) override;
    static Signature Deserialize(Deserialization& deserializer);
    bool operator==(const Signature& other) const;
    bool operator!=(const Signature& other) const;
    std::string ToString() const;
};

#endif //APTOS_SIGNATURE_H
