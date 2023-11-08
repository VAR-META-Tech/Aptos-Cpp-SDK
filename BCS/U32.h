//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_U32_H
#define APTOS_U32_H

#include "BCSTypes.h"
#include <cstdint>


class U32 : public ISerializableTag {
private:
    uint32_t value;

public:
    U32(uint32_t value);

    void Serialize(Serialization& serializer) const override;

    static uint32_t Deserialize(const std::vector<uint8_t>& data);

    static std::shared_ptr<ISerializableTag> Deserialize(Deserialization& deserializer);

    TypeTag Variant() const override;

    uint32_t GetValue() const;

    bool Equals(const U32& other) const;

    std::string ToString() const override;

    size_t GetHashCode() const;
};

#endif //APTOS_U32_H
