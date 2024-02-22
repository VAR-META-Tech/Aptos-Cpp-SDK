//
// Created by Anh NPH on 22/09/2023.
//

#include "BCSMap.h"
#include "Serialization.h"
#include "Deserialization.h"
#include <sstream>

namespace Aptos::BCS
{
    std::map<BString, std::shared_ptr<ISerializable>> BCSMap::getValues() const
    {
        return values;
    }

    BCSMap::BCSMap(const std::map<BString, std::shared_ptr<ISerializable>> &values)
        : values(values)
    {
    }

    void BCSMap::Serialize(Serialization &serializer) const
    {
        Serialization mapSerializer;
        std::map<std::string, std::pair<std::vector<uint8_t>, std::vector<uint8_t>>, std::less<>> byteMap;

        for (const auto &entry : this->values)
        {
            Serialization keySerializer;
            static_cast<BString>(entry.first).Serialize(keySerializer);
            std::vector<uint8_t> bKey = keySerializer.GetBytes();

            Serialization valSerializer;
            entry.second->Serialize(valSerializer);
            std::vector<uint8_t> bValue = valSerializer.GetBytes();

            byteMap.try_emplace(entry.first.GetValue(), std::make_pair(bKey, bValue));
        }
        mapSerializer.SerializeU32AsUleb128(byteMap.size());

        for (const auto &entry : byteMap)
        {
            mapSerializer.SerializeFixedBytes(entry.second.first);
            mapSerializer.SerializeFixedBytes(entry.second.second);
        }

        serializer.SerializeFixedBytes(mapSerializer.GetBytes());
    }

    std::string BCSMap::ToString() const
    {
        std::stringstream ss;
        for (const auto &entry : values)
            ss << "(" << entry.first.ToString() << ", " << entry.second->ToString() << ")";
        return ss.str();
    }
}