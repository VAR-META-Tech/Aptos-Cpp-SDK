//
// Created by Anh NPH on 21/09/2023.
//

#include "Bytes.h"

#include "Bytes.h"
#include <algorithm>
#include <sstream>
#include <functional>
#include "Serialization.h"
#include "Deserialization.h"

namespace Aptos::BCS
{
    Bytes::Bytes(const std::vector<uint8_t> &values): values(values)
    {
    }

    void Bytes::Serialize(Serialization &serializer) const
    {
        serializer.SerializeBytes(this->values);
    }

    std::shared_ptr<Bytes> Bytes::Deserialize(Deserialization &deserializer)
    {
        std::vector<uint8_t> values = deserializer.ToBytes();
        return std::make_shared<Bytes>(values);
    }

    std::vector<uint8_t> Bytes::getValue() const
    {
        return this->values;
    }

    bool Bytes::Equals(const Bytes &other) const
    {
        return std::equal(this->values.begin(), this->values.end(), other.values.begin());
    }

    std::string Bytes::ToString() const
    {
        std::ostringstream oss;
        for (const auto &value : values)
            oss << static_cast<int>(value);
        return oss.str();
    }

    size_t Bytes::GetHashCode() const
    {
        // Simple hash code calculation, more sophisticated method might be needed
        return std::hash<std::string>{}(this->ToString());
    }
}