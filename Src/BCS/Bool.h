//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_BOOL_H
#define APTOS_BOOL_H
#include "BCSTypes.h"

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a Boolean.
    /// </summary>
    class APTOS_API Bool : public ISerializableTag
    {
    private:
        /// <summary>
        /// The internal boolean value.
        /// </summary>
        bool value;

    public:
        /// <summary>
        /// Creates a Bool object from a given boolean.
        /// </summary>
        /// <param name="value">A bolean value to serialize.</param>
        explicit Bool(bool value);

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static std::shared_ptr<Bool> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        TypeTag Variant() const override;

        /// <inheritdoc/>
        bool GetValue() const;

        /// <inheritdoc/>
        bool Equals(const Bool &other) const;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <inheritdoc/>
        size_t GetHashCode() const override;
    };

    bool APTOS_API operator==(const Bool &lhs, const Bool &rhs);
}
#endif // APTOS_BOOL_H
