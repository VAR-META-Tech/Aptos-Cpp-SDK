//
// Created by Anh NPH on 21/09/2023.
//

#include "ByteSequence.h"
#include "Serialization.h"
#include "Deserialization.h"
#include <sstream>
#include <functional>

namespace Aptos::BCS
{
    BytesSequence::BytesSequence(const std::vector<std::vector<uint8_t>> &values)
        : values(values) {}

    void BytesSequence::Serialize(Serialization &serializer) const
    {
        serializer.SerializeU32AsUleb128(values.size());
        for (const auto &element : values)
            serializer.SerializeBytes(element);
    }

    std::shared_ptr<BytesSequence> BytesSequence::Deserialize(Deserialization &deserializer)
    {
        int length = deserializer.DeserializeUleb128();
        std::vector<std::vector<uint8_t>> bytesList;

        while (bytesList.size() < length)
            bytesList.push_back(deserializer.ToBytes());

        return std::make_shared<BytesSequence>(bytesList);
    }

    std::vector<std::vector<uint8_t>> BytesSequence::GetValue() const
    {
        return values;
    }

    bool BytesSequence::Equals(const BytesSequence &other) const
    {
        if (values.size() != other.values.size())
            return false;

        for (std::size_t i = 0; i < values.size(); ++i)
            if (values[i] != other.values[i])
                return false;

        return true;
    }

    std::string BytesSequence::ToString() const
    {
        std::stringstream ss;
        for (const auto &value : values)
            for (const auto &element : value)
                ss << std::hex << static_cast<int>(element);

        return ss.str();
    }

    size_t BytesSequence::GetHashCode() const
    {
        // Provide your hash function implementation here
        return std::hash<std::string>{}(ToString());
    }
}