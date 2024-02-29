#include "U8.h"
#include "Serialization.h"
#include "Deserialization.h"
#include <sstream>

namespace Aptos::BCS
{
    U8::U8(uint8_t value): value(value)
    {
    }

    void U8::Serialize(Serialization &serializer) const
    {
        serializer.SerializeU8(this->value);
    }

    std::shared_ptr<U8> U8::Deserialize(Deserialization &deserializer)
    {
        uint8_t value = deserializer.DeserializeU8();
        return std::make_shared<U8>(value);
    }

    TypeTag U8::Variant() const
    {
        return TypeTag::U8;
    }

    uint8_t U8::GetValue() const
    {
        return value;
    }

    bool U8::Equals(const U8 &other) const
    {
        return this->value == other.value;
    }

    std::string U8::ToString() const
    {
        std::ostringstream oss;
        oss << static_cast<int>(this->value);
        return oss.str();
    }

    size_t U8::GetHashCode() const
    {
        // Simple hash code calculation, more sophisticated method might be needed
        return std::hash<uint8_t>{}(this->value);
    }
}
