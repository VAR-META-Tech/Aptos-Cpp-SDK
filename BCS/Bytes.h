//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_BYTES_H
#define APTOS_BYTES_H
#include "BCSTypes.h"
#include <cstdint>

class Bytes : public ISerializable {
private:
    std::vector<uint8_t> values;

public:
    Bytes(const std::vector<uint8_t>& values);

    void Serialize(Serialization& serializer) const override;

    static Bytes* Deserialize(Deserialization& deserializer);

    std::vector<uint8_t> getValue() const;

    bool Equals(const Bytes& other) const;

    std::string ToString() const override;

    size_t GetHashCode() const;
};



#endif //APTOS_BYTES_H
