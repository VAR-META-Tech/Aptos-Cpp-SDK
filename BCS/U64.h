//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_U64_H
#define APTOS_U64_H

#include "BCSTypes.h"

class U64 : public ISerializableTag {
private:
    uint64_t value;

public:
    U64(uint64_t value);

    void Serialize(Serialization& serializer);

    static uint64_t Deserialize(const std::vector<uint8_t>& data);

    static U64* Deserialize(Deserialization& deserializer);

    TypeTag Variant();

    virtual void* GetValue();

    bool Equals(const U64& other) const;

    std::string ToString() const;

    size_t GetHashCode() const;
};
#endif //APTOS_U64_H
