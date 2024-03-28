#ifndef APTOS_U16_H
#define APTOS_U16_H
#include "BCSTypes.h"
#include <cstdint>

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a U32.
    /// </summary>
    class APTOS_API U16 : public ISerializableTag
    {
    private:
        /// <summary>
        /// The internal U16 values as a uint data type.
        /// </summary>
        uint16_t value;

    public:
        /// <summary>
        /// Creates a U16 object from a given uint value.
        /// </summary>
        /// <param name="value">A uint value to serialize as u16.</param>
        explicit U16(uint16_t value);

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static uint16_t Deserialize(const std::vector<uint8_t> &data);

        /// <inheritdoc/>
        static std::shared_ptr<U16> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        TypeTag Variant() const override;

        /// <inheritdoc/>
        uint16_t GetValue() const;

        /// <inheritdoc/>
        bool Equals(const U16 &other) const;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <inheritdoc/>
        size_t GetHashCode() const override;
    };
}
#endif // APTOS_U16_H
