//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_U8_H
#define APTOS_U8_H

#include "BCSTypes.h"

class U8 : public ISerializableTag {
private:
    uint8_t value;

public:
    U8(uint8_t value);

    void Serialize(Serialization& serializer);

    static U8* Deserialize(Deserialization& deserializer);

    TypeTag Variant();

    virtual void* GetValue();

    bool Equals(const U8& other) const;

    std::string ToString() const;

    size_t GetHashCode() const;
};

#endif //APTOS_U8_H
