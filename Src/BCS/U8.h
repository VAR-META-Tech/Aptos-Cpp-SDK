#ifndef APTOS_U8_H
#define APTOS_U8_H
#include "BCSTypes.h"
#include <cstdint>

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of U8.
    /// </summary>
    class APTOS_API U8 : public ISerializableTag
    {
    private:
        /// <summary>
        /// The internal U8 value as a byte.
        /// </summary>
        uint8_t value;

    public:
        /// <summary>
        /// Creates a U8 object from a given byte.
        /// </summary>
        /// <param name="value">A byte value to serialize as u8.</param>
        explicit U8(uint8_t value);

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static std::shared_ptr<U8> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        TypeTag Variant() const override;

        /// <inheritdoc/>
        uint8_t GetValue() const;

        /// <inheritdoc/>
        bool Equals(const U8 &other) const;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <inheritdoc/>
        size_t GetHashCode() const override;
    };
}
#endif // APTOS_U8_H
