//
// Created by Anh NPH on 22/09/2023.
//

#include "BString.h"
#include "Deserialization.h"
#include "Serialization.h"
#include <functional>

namespace Aptos::BCS
{
    BString::BString() : ISerializable()
    {
    }

    BString::BString(const std::string &value)
        : ISerializable(), value(value) {}

    void BString::Serialize(Serialization &serializer) const
    {
        serializer.Serialize(value);
    }

    std::shared_ptr<BString> BString::Deserialize(Deserialization &deserializer)
    {
        std::string deserStr = deserializer.DeserializeString();
        return std::make_shared<BString>(deserStr);
    }

    bool BString::Equals(const BString &other) const
    {
        return this->value == other.value;
    }

    std::string BString::ToString() const
    {
        return value;
    }

    size_t BString::GetHashCode() const
    {
        return std::hash<std::string>{}(value);
    }

    std::string BString::GetValue() const
    {
        return value;
    }

    std::vector<uint8_t> BString::RemoveBOM(const std::vector<uint8_t> &data)
    {
        std::vector<uint8_t> bom = {0xEF, 0xBB, 0xBF}; // UTF-8 BOM

        if (data.size() > bom.size())
        {
            if (std::equal(bom.begin(), bom.end(), data.begin()))
            {
                return std::vector<uint8_t>(data.begin() + bom.size(), data.end());
            }
        }

        return data;
    }

    bool BString::operator<(const BString &other) const
    {
        return this->value < other.value;
    }

    std::string BString::Deserialize(const std::vector<uint8_t> &data)
    {
        return std::string(data.begin(), data.end());
    }

    bool BString::operator==(const BString &other) const
    {
        return this->value == other.value;
    }
}