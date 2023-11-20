#include "U16.h"
#include "Serialization.h"
#include "Deserialization.h"
#include <boost/endian/conversion.hpp>
#include <sstream>

namespace Aptos::BCS
{
    U16::U16(uint16_t value)
    {
        this->value = value;
    }

    void U16::Serialize(Serialization &serializer) const
    {
        serializer.SerializeU16(this->value);
    }

    std::shared_ptr<ISerializableTag> U16::Deserialize(Deserialization &deserializer)
    {
        uint16_t value = deserializer.DeserializeU16();
        return std::make_shared<U16>(value);
    }

    TypeTag U16::Variant() const
    {
        return TypeTag::U8;
    }

    uint16_t U16::GetValue() const
    {
        return value;
    }

    bool U16::Equals(const U16 &other) const
    {
        return this->value == other.value;
    }

    std::string U16::ToString() const
    {
        std::ostringstream oss;
        oss << static_cast<int>(this->value);
        return oss.str();
    }

    size_t U16::GetHashCode() const
    {
        // Simple hash code calculation, more sophisticated method might be needed
        return std::hash<uint16_t>{}(this->value);
    }

    uint16_t U16::Deserialize(const std::vector<uint8_t> &data)
    {
        if (data.size() < sizeof(uint16_t))
        {
            throw std::runtime_error("Not enough bytes to deserialize a uint16_t");
        }

        uint16_t res;
        std::memcpy(&res, data.data(), sizeof(uint16_t));
        return boost::endian::little_to_native(res);
    }
}