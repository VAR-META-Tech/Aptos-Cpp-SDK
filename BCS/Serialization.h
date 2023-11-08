//
// Created by Anh NPH on 18/09/2023.
//

#ifndef APTOS_SERIALIZATION_H
#define APTOS_SERIALIZATION_H

#include <string>
#include <vector>
#include "BCSTypes.h"
#include <cryptopp/integer.h>

class Serialization {
public:
    std::vector<uint8_t> GetBytes();

    Serialization& Serialize(const std::string& value);

    Serialization& Serialize(const std::vector<uint8_t>& value);

    Serialization& Serialize(bool value);

    Serialization& Serialize(uint8_t num);

    Serialization& Serialize(uint16_t num);

    Serialization& Serialize(uint32_t num);

    Serialization& Serialize(uint64_t num);

    Serialization& Serialize(CryptoPP::Integer num);

    Serialization& Serialize(const ISerializable &value);

    Serialization& Serialize(std::vector<std::shared_ptr<ISerializable> > args);

    Serialization& SerializeString(const std::string& value);

    Serialization& SerializeBytes(const std::vector<uint8_t>& bytes);

    Serialization& SerializeFixedBytes(const std::vector<uint8_t>& bytes);

    Serialization& SerializeU32AsUleb128(uint32_t value);

    Serialization& SerializeBool(bool value);

    Serialization& SerializeU8(uint8_t num);

    Serialization& SerializeU16(uint16_t num);

    Serialization& SerializeU32(uint32_t num);

    Serialization& SerializeU64(uint64_t num);

    Serialization& SerializeU128(CryptoPP::Integer num);

    Serialization& SerializeU256(CryptoPP::Integer num);

private:
    std::vector<uint8_t> buffer;
};


#endif //APTOS_SERIALIZATION_H
