// Deserialization.cpp
#include "Deserialization.h"
#include "Sequence.h"
#include "Serialization.h"
// #include "BCS.h"
#include <algorithm>
#include "U16.h"
#include "U32.h"
#include "U64.h"
#include "U128.h"
#include "U256.h"
#include "BString.h"
#include "TagSequence.h"
#include "ScriptArgument/ScriptArgument.h"

namespace Aptos::BCS
{
    Deserialization::Deserialization(const std::vector<uint8_t> &data) : buffer(data)
    {
    }

    std::size_t Deserialization::Remaining() const
    {
        return buffer.size() - position;
    }

    bool Deserialization::DeserializeBool()
    {
        auto bytes = Read(1);
        return bytes[0] != 0;
    }

    std::vector<uint8_t> Deserialization::ToBytes()
    {
        auto len = DeserializeUleb128();
        return Read(len);
    }

    std::vector<uint8_t> Deserialization::FixedBytes(std::size_t length)
    {
        return Read(length);
    }

    std::vector<uint8_t> Deserialization::Read(std::size_t length)
    {
        if (position + length > buffer.size())
        {
            throw std::runtime_error("Unexpected end of input.");
        }

        std::vector<uint8_t> result(buffer.begin() + position, buffer.begin() + position + length);
        position += length;
        return result;
    }

    uint8_t Deserialization::ReadInt(std::size_t length)
    {
        auto bytes = Read(length);
        return bytes[0];
    }

    uint8_t Deserialization::DeserializeU8()
    {
        return ReadInt(1);
    }

    uint16_t Deserialization::DeserializeU16()
    {
        return U16::Deserialize(this->Read(2));
    }
    uint32_t Deserialization::DeserializeU32()
    {
        return U32::Deserialize(this->Read(4));
    }
    uint64_t Deserialization::DeserializeU64()
    {
        return U64::Deserialize(this->Read(8));
    }

    std::size_t Deserialization::DeserializeUleb128()
    {
        std::size_t value = 0;
        int shift = 0;

        while (true)
        {
            uint8_t byteRead = ReadInt(1);
            value |= (byteRead & 0x7F) << shift;

            if ((byteRead & 0x80) == 0)
                break;
            shift += 7;
        }

        if (value > std::pow(2, 128) - 1)
        {
            throw std::runtime_error("Unexpectedly large uleb128 value");
        }

        return value;
    }

    CryptoPP::Integer Deserialization::DeserializeU128()
    {
        return U128::Deserialize(this->Read(16));
    }

    CryptoPP::Integer Deserialization::DeserializeU256()
    {
        return U256::Deserialize(this->Read(32));
    }

    std::string Deserialization::DeserializeString()
    {
        return BString::Deserialize(this->Read(this->DeserializeUleb128()));
    }

    BCSMap Deserialization::DeserializeMap(std::function<std::shared_ptr<ISerializable>(Deserialization &)> deserializeFunc)
    {
        // Read the number of key-value pairs
        uint32_t numPairs = this->DeserializeUleb128();

        std::map<std::string, std::shared_ptr<ISerializable>, std::less<>> sortedMap;

        // Deserialize each key-value pair
        for (uint32_t i = 0; i < numPairs; i++)
        {
            // Deserialize the key
            std::string key = this->DeserializeString();

            // Add the key-value pair to the map
            sortedMap[key] = deserializeFunc(*this);
        }

        std::map<BString, std::shared_ptr<ISerializable>> values;
        for (const auto &entry : sortedMap)
        {
            values[BString(entry.first)] = entry.second;
        }

        // Construct a new BCSMap object and return it
        return BCSMap(values);
    }

    std::vector<std::shared_ptr<ISerializable>> Deserialization::DeserializeSequence(std::function<std::shared_ptr<ISerializable>(Deserialization &)> deserializeFunc)
    {
        // Read the number of values
        uint32_t numPairs = this->DeserializeUleb128();

        std::vector<std::shared_ptr<ISerializable>> values;

        // Deserialize each value
        for (uint32_t i = 0; i < numPairs; i++)
        {
            // Deserialize the value
            values.push_back(deserializeFunc(*this));
        }

        return values;
    }

    std::shared_ptr<TagSequence> Deserialization::DeserializeTagSequence()
    {
        int length = DeserializeUleb128();
        std::vector<std::shared_ptr<ISerializableTag>> values;

        while (values.size() < length)
        {
            std::shared_ptr<ISerializableTag> val = ISerializableTag::DeserializeTag(*this);
            values.push_back(val);
        }

        return std::make_shared<TagSequence>(values);
    }

    std::shared_ptr<Sequence> Deserialization::DeserializeScriptArgSequence()
    {
        int length = DeserializeUleb128();
        std::vector<std::shared_ptr<ISerializable>> values;

        while (values.size() < length)
        {
            std::shared_ptr<ISerializable> val = ScriptArgument::Deserialize(*this);
            values.push_back(val);
        }
        return std::make_shared<Sequence>(values);
    }
}