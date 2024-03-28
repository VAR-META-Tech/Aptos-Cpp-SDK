//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_BYTESEQUENCE_H
#define APTOS_BYTESEQUENCE_H
#include "BCSTypes.h"

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a byte sequence.
    /// </summary>
    class APTOS_API BytesSequence : public ISerializable
    {
    private:
        /// <summary>
        /// A list of a list of bytes.
        /// </summary>
        std::vector<std::vector<uint8_t>> values;

    public:
        /// <summary>
        /// Creates a ByteSequence object from a list of a list of bytes.
        /// </summary>
        /// <param name="values">A lsit of a list of bytes.</param>
        explicit BytesSequence(const std::vector<std::vector<uint8_t>> &values);

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static std::shared_ptr<BytesSequence> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        std::vector<std::vector<uint8_t>> GetValue() const;

        /// <inheritdoc/>
        bool Equals(const BytesSequence &other) const;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <inheritdoc/>
        size_t GetHashCode() const override;
    };
}
#endif // APTOS_BYTESEQUENCE_H
