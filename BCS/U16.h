//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_U16_H
#define APTOS_U16_H

#include "BCSTypes.h"
#include <cstdint>

class U16 : public ISerializableTag {
private:
    uint16_t value;

public:
    U16(uint16_t value);

    void Serialize(Serialization& serializer) const override;

    static uint16_t Deserialize(const std::vector<uint8_t>& data);

    static std::shared_ptr<ISerializableTag> Deserialize(Deserialization& deserializer);

    TypeTag Variant() const override;

    uint16_t GetValue() const;

    bool Equals(const U16& other) const;

    std::string ToString() const override;

    size_t GetHashCode() const;
};
#endif //APTOS_U16_H
