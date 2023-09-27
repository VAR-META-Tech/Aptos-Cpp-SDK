//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_BYTES_H
#define APTOS_BYTES_H
#include "BCSTypes.h"

class Bytes : public ISerializable {
private:
    std::vector<uint8_t> values;

public:
    Bytes(const std::vector<uint8_t>& values);

    void Serialize(Serialization& serializer);

    static Bytes* Deserialize(Deserialization& deserializer);

    std::vector<uint8_t> getValue();

    bool Equals(const Bytes& other) const;

    std::string ToString() const;

    size_t GetHashCode() const;
};



#endif //APTOS_BYTES_H
