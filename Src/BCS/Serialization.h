//
// Created by Anh NPH on 18/09/2023.
//

#ifndef APTOS_SERIALIZATION_H
#define APTOS_SERIALIZATION_H

#include <string>
#include <vector>
#include "BCSTypes.h"
#include <cryptopp/integer.h>
#include "Sequence.h"

namespace Aptos::BCS
{
    /// <summary>
    /// An implementation of BCS Serialization in C#
    /// </summary>
    class APTOS_API Serialization
    {
    public:
        /// <summary>
        /// Return the serialization buffer as a byte array.
        /// </summary>
        /// <returns>Serialization buffer as a byte array.</returns>
        std::vector<uint8_t> GetBytes();

        /// <summary>
        /// Serialize a string value.
        /// </summary>
        /// <param name="value">String value to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &Serialize(const std::string &value);

        /// <summary>
        /// Serialize a byte array.
        /// </summary>
        /// <param name="value">Byte array to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &Serialize(const std::vector<uint8_t> &value);

        /// <summary>
        /// Serialize a boolean value.
        /// </summary>
        /// <param name="value">Boolean value to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &Serialize(bool value);

        /// <summary>
        /// Serialize a single byte
        /// </summary>
        /// <param name="num">Byte to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &Serialize(uint8_t num);

        /// <summary>
        /// Serialize an unsigned short value.
        /// </summary>
        /// <param name="num">The number to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &Serialize(uint16_t num);

        /// <summary>
        /// Serialize an unsigned integer value.
        /// </summary>
        /// <param name="num">The unsigned integer to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &Serialize(uint32_t num);

        /// <summary>
        /// Serialize an unsigned long number.
        /// </summary>
        /// <param name="num">The unsigned long number to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &Serialize(uint64_t num);

        /// <summary>
        /// Serialize a big integer number.
        /// </summary>
        /// <param name="num">The big integer number to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &Serialize(CryptoPP::Integer num);

        /// <summary>
        /// Serializes an object using it's own serialization implementation.
        /// </summary>
        /// <param name="value">Value to serialize</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &Serialize(const ISerializable &value);

        /// <summary>
        /// Serializes an array of serializable elements
        /// </summary>
        /// <param name="args"></param>
        /// <returns></returns>
        Serialization &Serialize(std::vector<std::shared_ptr<ISerializable>> args);
        Serialization &Serialize(const Sequence &args);

        /// <summary>
        /// Serializes a string. UTF8 string is supported. Serializes the string's bytes length "l" first,
        /// and then serializes "l" bytes of the string content.
        ///
        /// BCS layout for "string": string_length | string_content. string_length is the bytes length of
        /// the string that is uleb128 encoded. string_length is a u32 integer.
        /// </summary>
        /// <param name="value">String value to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &SerializeString(const std::string &value);

        /// <summary>
        /// Serializes an array of bytes.
        /// BCS layout for "bytes": bytes_length | bytes. bytes_length is the length of the bytes array that is
        /// uleb128 encoded. bytes_length is a u32 integer.
        /// </summary>
        /// <param name="bytes">Byte array to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &SerializeBytes(const std::vector<uint8_t> &bytes);

        /// <summary>
        /// Serializes a list of values represented in a byte array.
        /// This can be a sequence or a value represented as a byte array.
        /// Note that for sequences we first add the length for the entire sequence array,
        /// not the length of the byte array.
        /// </summary>
        /// <param name="bytes">Byte array to be serialized.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &SerializeFixedBytes(const std::vector<uint8_t> &bytes);

        /// <summary>
        /// Serialize an unsigned integer value.
        /// Usually used to serialize the length of values.
        /// </summary>
        /// <param name="value">Unsigned integer to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &SerializeU32AsUleb128(uint32_t value);

        /// <summary>
        /// Serialize a boolean value
        /// </summary>
        /// <param name="value">Boolean value to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &SerializeBool(bool value);

        /// <summary>
        /// Serialize a unsigned byte number.
        /// </summary>
        /// <param name="num">Byte to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &SerializeU8(uint8_t num);

        /// <summary>
        /// Serialize unsigned short (U16).
        /// </summary>
        /// <param name="num">Unsigned short number to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &SerializeU16(uint16_t num);

        /// <summary>
        /// Serialize an unsigned integer number.
        /// </summary>
        /// <param name="num">Unsigned integer number.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &SerializeU32(uint32_t num);

        /// <summary>
        /// Serialize an unsigned long number.
        /// </summary>
        /// <param name="num">Unsigned long number to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &SerializeU64(uint64_t num);

        /// <summary>
        /// Serialize a big integer value.
        /// </summary>
        /// <param name="num">Big integer value to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &SerializeU128(CryptoPP::Integer num);

        /// <summary>
        /// Serialize a unsigned 256 int (big integer) value.
        /// </summary>
        /// <param name="num">Big integer value to serialize.</param>
        /// <returns>The current Serialization object.</returns>
        Serialization &SerializeU256(CryptoPP::Integer num);

    private:
        std::vector<uint8_t> buffer;
    };
}
#endif // APTOS_SERIALIZATION_H
