#ifndef APTOS_U32_H
#define APTOS_U32_H
#include "BCSTypes.h"
#include <cstdint>

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a U32.
    /// </summary>
    class APTOS_API U32 : public ISerializableTag
    {
    private:
        /// <summary>
        /// The internal U32 values as a uint data type.
        /// </summary>
        uint32_t value;

    public:
        /// <summary>
        /// Creates a U32 object from a uint value.
        /// </summary>
        /// <param name="value">A uint value to serialize as u32.</param>
        explicit U32(uint32_t value);

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static uint32_t Deserialize(const std::vector<uint8_t> &data);

        /// <inheritdoc/>
        static std::shared_ptr<U32> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        TypeTag Variant() const override;

        /// <inheritdoc/>
        uint32_t GetValue() const;

        /// <inheritdoc/>
        bool Equals(const U32 &other) const;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <inheritdoc/>
        size_t GetHashCode() const override;
    };
}
#endif // APTOS_U32_H
