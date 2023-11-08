//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_U8_H
#define APTOS_U8_H

#include "BCSTypes.h"
#include <cstdint>

class U8 : public ISerializableTag {
private:
    uint8_t value;

public:
    U8(uint8_t value);
    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializableTag> Deserialize(Deserialization& deserializer);
    TypeTag Variant() const override;
    uint8_t GetValue() const;
    bool Equals(const U8& other) const;
    std::string ToString() const override;
    size_t GetHashCode() const;
};

#endif //APTOS_U8_H
