#include "U32.h"
#include "Serialization.h"
#include "Deserialization.h"
#include <sstream>
#include <stdexcept>
#include <bit>

namespace Aptos::BCS
{
    U32::U32(uint32_t value): value(value)
    {
    }

    void U32::Serialize(Serialization &serializer) const
    {
        serializer.SerializeU32(this->value);
    }

    std::shared_ptr<U32> U32::Deserialize(Deserialization &deserializer)
    {
        uint32_t value = deserializer.DeserializeU32();
        return std::make_shared<U32>(value);
    }

    TypeTag U32::Variant() const
    {
        return TypeTag::U32;
    }

    uint32_t U32::GetValue() const
    {
        return value;
    }

    bool U32::Equals(const U32 &other) const
    {
        return this->value == other.value;
    }

    std::string U32::ToString() const
    {
        std::ostringstream oss;
        oss << static_cast<int>(this->value);
        return oss.str();
    }

    size_t U32::GetHashCode() const
    {
        // Simple hash code calculation, more sophisticated method might be needed
        return std::hash<uint32_t>{}(this->value);
    }

    uint32_t U32::Deserialize(const std::vector<uint8_t> &data)
    {
        if (data.size() < sizeof(uint32_t))
        {
            throw std::runtime_error("Not enough bytes to deserialize a uint32_t");
        }

        uint32_t res;
        std::memcpy(&res, data.data(), sizeof(uint32_t));
        if constexpr (std::endian::native == std::endian::little)
        {
            // The native endianness is little endian, no conversion needed.
            return res;
        }
        else if constexpr (std::endian::native == std::endian::big)
        {
            // The native endianness is big endian, conversion needed.
            // Since there is no std::byteswap in C++20, we use inline byte swap logic.
            return (res >> 24) |
                   ((res & 0x00FF0000) >> 8) |
                   ((res & 0x0000FF00) << 8) |
                   (res << 24);
        }
    }
}
