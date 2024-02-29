//
// Created by Anh NPH on 18/09/2023.
//
#include "Serialization.h"
#include <algorithm>
#include <filesystem>

namespace Aptos::BCS
{
    std::vector<uint8_t> Serialization::GetBytes()
    {
        return buffer;
    }

    Serialization &Serialization::Serialize(const std::string &value)
    {
        SerializeString(value);
        return *this;
    }

    Serialization &Serialization::Serialize(const std::vector<uint8_t> &value)
    {
        SerializeBytes(value);
        return *this;
    }

    Serialization &Serialization::Serialize(bool value)
    {
        SerializeBool(value);
        return *this;
    }

    Serialization &Serialization::Serialize(uint8_t num)
    {
        SerializeU8(num);
        return *this;
    }

    Serialization &Serialization::Serialize(uint16_t num)
    {
        SerializeU16(num);
        return *this;
    }

    Serialization &Serialization::Serialize(uint32_t num)
    {
        SerializeU32(num);
        return *this;
    }

    Serialization &Serialization::Serialize(uint64_t num)
    {
        SerializeU64(num);
        return *this;
    }

    Serialization &Serialization::Serialize(CryptoPP::Integer num)
    {
        SerializeU128(num);
        return *this;
    }

    Serialization &Serialization::Serialize(const ISerializable &value)
    {
        value.Serialize(*this);
        return *this;
    }

    Serialization &Serialization::SerializeString(const std::string &value)
    {
        std::vector<uint8_t> utf8_bytes(value.begin(), value.end());
        SerializeBytes(utf8_bytes);
        return *this;
    }

    Serialization &Serialization::SerializeBytes(const std::vector<uint8_t> &bytes)
    {
        SerializeU32AsUleb128(bytes.size());
        buffer.insert(buffer.end(), bytes.begin(), bytes.end());
        return *this;
    }

    Serialization &Serialization::SerializeFixedBytes(const std::vector<uint8_t> &bytes)
    {
        buffer.insert(buffer.end(), bytes.begin(), bytes.end());
        return *this;
    }

    Serialization &Serialization::SerializeU32AsUleb128(uint32_t value)
    {
        while (value >= 0x80)
        {
            uint8_t b = value & 0x7f;
            buffer.push_back(b | 0x80);
            value >>= 7;
        }

        buffer.push_back(value & 0x7f);
        return *this;
    }

    Serialization &Serialization::SerializeBool(bool value)
    {
        buffer.push_back(value ? 0x01 : 0x00);
        return *this;
    }

    Serialization &Serialization::SerializeU8(uint8_t num)
    {
        buffer.push_back(num);
        return *this;
    }

    Serialization &Serialization::SerializeU16(uint16_t num)
    {
        uint8_t lower = num & 0xFF;
        uint8_t upper = (num >> 8) & 0xFF;
        buffer.push_back(lower); // Store the lower byte first (little-endian order)
        buffer.push_back(upper); // Store the upper byte second
        return *this;
    }

    Serialization &Serialization::SerializeU32(uint32_t num)
    {
        uint8_t b1 = (num & 0xFF);
        uint8_t b2 = (num >> 8) & 0xFF;
        uint8_t b3 = (num >> 16) & 0xFF;
        uint8_t b4 = (num >> 24) & 0xFF;
        buffer.push_back(b1);
        buffer.push_back(b2);
        buffer.push_back(b3);
        buffer.push_back(b4);
        return *this;
    }

    Serialization &Serialization::SerializeU64(uint64_t num)
    {
        uint8_t b1 = num & 0xFF;
        uint8_t b2 = (num >> 8) & 0xFF;
        uint8_t b3 = (num >> 16) & 0xFF;
        uint8_t b4 = (num >> 24) & 0xFF;
        uint8_t b5 = (num >> 32) & 0xFF;
        uint8_t b6 = (num >> 40) & 0xFF;
        uint8_t b7 = (num >> 48) & 0xFF;
        uint8_t b8 = (num >> 56) & 0xFF;
        buffer.push_back(b1);
        buffer.push_back(b2);
        buffer.push_back(b3);
        buffer.push_back(b4);
        buffer.push_back(b5);
        buffer.push_back(b6);
        buffer.push_back(b7);
        buffer.push_back(b8);
        return *this;
    }

    Serialization &Serialization::SerializeU128(CryptoPP::Integer num)
    {
        std::vector<uint8_t> content(16, 0); // Initialize with 16 zero bytes

        num.Encode(content.data(), 16);

        // convert from big endian to little endian
        std::reverse(content.begin(), content.end());

        buffer.insert(buffer.end(), content.begin(), content.end());

        return *this;
    }

    Serialization &Serialization::SerializeU256(CryptoPP::Integer num)
    {
        std::vector<uint8_t> content(32, 0); // Initialize with 16 zero bytes

        num.Encode(content.data(), 32);

        // convert from big endian to little endian
        std::reverse(content.begin(), content.end());

        buffer.insert(buffer.end(), content.begin(), content.end());

        return *this;
    }

    Serialization &Serialization::Serialize(std::vector<std::shared_ptr<ISerializable>> args)
    {
        SerializeU32AsUleb128(static_cast<uint32_t>(args.size()));
        for (auto &element : args)
        {
            Serialization s;
            element->Serialize(s);
            std::vector<uint8_t> b = s.GetBytes();
            SerializeFixedBytes(b);
        }
        return *this;
    }

    Serialization &Serialization::Serialize(const Sequence &args)
    {
        const auto& vl= args.getValues();
        SerializeU32AsUleb128(static_cast<uint32_t>(vl.size()));
        for (auto &element : vl)
        {
            Serialization s;
            element->Serialize(s);
            std::vector<uint8_t> b = s.GetBytes();
            SerializeFixedBytes(b);
        }
        return *this;
    }
}