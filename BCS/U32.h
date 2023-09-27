//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_U32_H
#define APTOS_U32_H

#include "BCSTypes.h"

class U32 : public ISerializableTag {
private:
    uint32_t value;

public:
    U32(uint32_t value);

    void Serialize(Serialization& serializer);

    static uint32_t Deserialize(const std::vector<uint8_t>& data);

    static U32* Deserialize(Deserialization& deserializer);

    TypeTag Variant();

    virtual void* GetValue();

    bool Equals(const U32& other) const;

    std::string ToString() const;

    size_t GetHashCode() const;
};

#endif //APTOS_U32_H
