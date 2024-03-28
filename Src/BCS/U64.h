#ifndef APTOS_U64_H
#define APTOS_U64_H
#include "BCSTypes.h"

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of U64.
    /// </summary>
    class APTOS_API U64 : public ISerializableTag
    {
    private:
        /// <summary>
        /// The internal U64 value as a ulong data type.
        /// </summary>
        uint64_t value;

    public:
        /// <summary>
        /// Creates a U64 object from a given ulong value.
        /// </summary>
        /// <param name="value">A ulong value to serialize as u64.</param>
        explicit U64(uint64_t value);

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static uint64_t Deserialize(const std::vector<uint8_t> &data);

        /// <inheritdoc/>
        static std::shared_ptr<U64> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        TypeTag Variant() const override;

        /// <inheritdoc/>
        uint64_t GetValue() const;

        /// <inheritdoc/>
        bool Equals(const U64 &other) const;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <inheritdoc/>
        size_t GetHashCode() const override;
    };
}
#endif // APTOS_U64_H
