#include "U64.h"
#include "Serialization.h"
#include "Deserialization.h"
#include <boost/endian/conversion.hpp>
#include <sstream>

namespace Aptos::BCS
{
    U64::U64(uint64_t value)
    {
        this->value = value;
    }

    void U64::Serialize(Serialization &serializer) const
    {
        serializer.SerializeU64(this->value);
    }

    std::shared_ptr<ISerializableTag> U64::Deserialize(Deserialization &deserializer)
    {
        uint64_t value = deserializer.DeserializeU64();
        return std::make_shared<U64>(value);
    }

    TypeTag U64::Variant() const
    {
        return TypeTag::U64;
    }

    uint64_t U64::GetValue() const
    {
        return value;
    }

    bool U64::Equals(const U64 &other) const
    {
        return this->value == other.value;
    }

    std::string U64::ToString() const
    {
        std::ostringstream oss;
        oss << static_cast<int>(this->value);
        return oss.str();
    }

    size_t U64::GetHashCode() const
    {
        // Simple hash code calculation, more sophisticated method might be needed
        return std::hash<uint64_t>{}(this->value);
    }

    uint64_t U64::Deserialize(const std::vector<uint8_t> &data)
    {
        if (data.size() < sizeof(uint64_t))
        {
            throw std::runtime_error("Not enough bytes to deserialize a uint64_t");
        }

        uint64_t res;
        std::memcpy(&res, data.data(), sizeof(uint64_t));
        return boost::endian::little_to_native(res);
    }
}
