#ifndef APTOS_U128_H
#define APTOS_U128_H

#include "BCSTypes.h"
#include <cryptopp/integer.h>

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a U128.
    /// </summary>
    class APTOS_API U128 : public ISerializableTag
    {
    private:
        /// <summary>
        /// The internal U128 value as a BigInteger data type.
        /// </summary>
        CryptoPP::Integer value;

    public:
        /// <summary>
        /// Creates a U128 objeect from a BigInteger value.
        /// </summary>
        /// <param name="value">A BigInteger value to serialize as u128.</param>
        explicit U128(CryptoPP::Integer value);

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static std::shared_ptr<U128> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        TypeTag Variant() const override;

        /// <inheritdoc/>
        CryptoPP::Integer GetValue() const;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <inheritdoc/>
        bool Equals(const U128 &other) const;

        /// <inheritdoc/>
        size_t GetHashCode() const override;

        /// <inheritdoc/>
        static CryptoPP::Integer Deserialize(const std::vector<uint8_t> &data);
    };
}
#endif // APTOS_U128_H
