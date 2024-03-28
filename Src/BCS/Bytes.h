//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_BYTES_H
#define APTOS_BYTES_H
#include "BCSTypes.h"
#include <cstdint>

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of Bytes in BCS.
    /// </summary>
    class APTOS_API Bytes : public ISerializable
    {
    private:
        /// <summary>
        /// The internals byte array.
        /// </summary>
        std::vector<uint8_t> values;

    public:
        /// <summary>
        /// Creates a Bytes object from a given byte array.
        /// </summary>
        /// <param name="values">A list of bytes to serialize.</param>
        explicit Bytes(const std::vector<uint8_t> &values);

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static std::shared_ptr<Bytes> Deserialize(Deserialization &deserializer);

        /// <summary>
        /// Gets the byte array containes within the Bytes object.
        /// </summary>
        /// <returns></returns>
        std::vector<uint8_t> getValue() const;

        /// <inheritdoc/>
        bool Equals(const Bytes &other) const;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <inheritdoc/>
        size_t GetHashCode() const override;
    };
}
#endif // APTOS_BYTES_H
