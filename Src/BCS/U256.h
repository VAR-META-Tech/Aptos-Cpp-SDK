#ifndef APTOS_U256_H
#define APTOS_U256_H

#include "BCSTypes.h"
#include <cryptopp/integer.h>

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a 256.
    /// </summary>
    class APTOS_API U256 : public ISerializableTag
    {
    private:
        /// <summary>
        /// The internal U256 value as a BigInteger data type.
        /// </summary>
        CryptoPP::Integer value;

    public:
        /// <summary>
        /// Creates a U256 object from a given BigInteger value.
        /// </summary>
        /// <param name="value">A BigInteger value to serialize as u256.</param>
        explicit U256(CryptoPP::Integer value);

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static std::shared_ptr<U256> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        TypeTag Variant() const override;

        /// <inheritdoc/>
        CryptoPP::Integer GetValue() const;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <inheritdoc/>
        bool Equals(const U256 &other) const;

        /// <inheritdoc/>
        size_t GetHashCode() const override;

        /// <inheritdoc/>
        static CryptoPP::Integer Deserialize(const std::vector<uint8_t> &data);
    };
}
#endif // APTOS_U256_H
