//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_SEQUENCE_H
#define APTOS_SEQUENCE_H
#include "BCSTypes.h"
#include <memory>

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a Transaction Argument sequence / list.
    /// NOTE: Transaction Arguments have different types hence they cannot be represented using a regular list.
    /// NOTE: This class does not implement deserialization because the developer would know the types beforehand,
    /// and hence would apply the appropriate deserialization based on the type.
    ///
    /// Fixed and Variable Length Sequences
    /// Sequences can be made of up of any BCS supported types(even complex structures)
    /// but all elements in the sequence must be of the same type.If the length of a sequence
    /// is fixed and well known then BCS represents this as just the concatenation of the
    /// serialized form of each individual element in the sequence. If the length of the sequence
    /// can be variable, then the serialized sequence is length prefixed with a ULEB128-encoded unsigned integer
    /// indicating the number of elements in the sequence. All variable length sequences must
    /// be MAX_SEQUENCE_LENGTH elements long or less.
    /// </summary>
    class APTOS_API Sequence : public ISerializable
    {

    public:
        /// <summary>
        /// Creates a Sequence object from a list of serializable objects,
        /// e.g. U8, AccountAddress.
        /// </summary>
        /// <param name="serializable">A list of serializable objects.</param>
        explicit Sequence(const std::vector<std::shared_ptr<ISerializable>> &values);
        Sequence() = default;

        /// <summary>
        /// The length of the Sequence.
        /// </summary>
        int Length() const;

        /// <summary>
        /// Gets the internal list of objects inside the Sequence.
        /// </summary>
        /// <returns>The list of object.</returns>
        std::vector<std::shared_ptr<ISerializable>> GetValue() const;

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static std::shared_ptr<Sequence> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        bool Equals(const Sequence &other) const;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <inheritdoc/>
        size_t GetHashCode() const override;

        /// <inheritdoc/>
        const std::vector<std::shared_ptr<ISerializable>> &getValues() const;

    private:
        /// <summary>
        /// The internal list of objects that are to be serialized or deserialized.
        /// </summary>
        std::vector<std::shared_ptr<ISerializable>> values;
    };
}
#endif // APTOS_SEQUENCE_H
