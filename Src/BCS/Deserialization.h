//
// Created by Anh NPH on 18/09/2023.
//

#ifndef APTOS_DESERIALIZATION_H
#define APTOS_DESERIALIZATION_H

// Deserialization.h
#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include "BCSTypes.h"
#include "BCSMap.h"
#include <cryptopp/integer.h>
#include "Sequence.h"
#include "TagSequence.h"

namespace Aptos::BCS
{
    class APTOS_API Deserialization
    {
    public:
        explicit Deserialization(const std::vector<uint8_t> &data);

        std::size_t Remaining() const;
        bool DeserializeBool();
        std::vector<uint8_t> ToBytes();
        std::vector<uint8_t> FixedBytes(std::size_t length);
        BCSMap DeserializeMap(std::function<std::shared_ptr<ISerializable>(Deserialization &)> deserializeFunc);

        /// <summary>
        /// Deserializes a list of objects of same type
        /// </summary>
        /// <param name="valueDecoderType"></param>
        /// <returns></returns>
        std::vector<std::shared_ptr<ISerializable>> DeserializeSequence(
            std::function<std::shared_ptr<ISerializable>(Deserialization &)> deserializeFunc);
        std::shared_ptr<TagSequence> DeserializeTagSequence();
        std::shared_ptr<Sequence> DeserializeScriptArgSequence();
        std::vector<Sequence> DeserializeArgSequence();
        std::string DeserializeString();
        ISerializable DeserializeStruct();
        uint8_t DeserializeU8();
        uint16_t DeserializeU16();
        uint32_t DeserializeU32();
        uint64_t DeserializeU64();
        CryptoPP::Integer DeserializeU128();
        CryptoPP::Integer DeserializeU256();
        std::size_t DeserializeUleb128();

    private:
        std::vector<uint8_t> Read(std::size_t length);
        uint8_t ReadInt(std::size_t length);
        std::vector<uint8_t> buffer;
        std::size_t position = 0;
    };
}
#endif // APTOS_DESERIALIZATION_H
