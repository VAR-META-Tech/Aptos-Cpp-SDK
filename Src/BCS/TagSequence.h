#ifndef APTOS_TAGSEQUENCE_H
#define APTOS_TAGSEQUENCE_H
#include "BCSTypes.h"

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a tag sequence.
    /// </summary>
    class APTOS_API TagSequence : public ISerializable
    {
    public:
        /// <summary>
        /// Creates a TagSequence objects from a list of serializable tags.
        /// </summary>
        /// <param name="serializableTags">A list of serializable tags.</param>
        explicit TagSequence(const std::vector<std::shared_ptr<ISerializableTag>> &serializableTags);
        TagSequence() = default;
        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static std::shared_ptr<TagSequence> Deserialize(Deserialization &deserializer);

        /// <summary>
        /// Gets the internal list of objects inside the TagSequence.
        /// </summary>
        /// <returns>The list of objects.</returns>
        std::vector<std::shared_ptr<ISerializableTag>> GetValue() const;

        /// <inheritdoc/>
        bool Equals(const TagSequence &other) const;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <inheritdoc/>
        size_t GetHashCode() const override;

    private:
        /// <summary>
        /// A list of serializable tags.
        /// </summary>
        std::vector<std::shared_ptr<ISerializableTag>> serializableTags;
    };

    bool APTOS_API operator==(const TagSequence &lhs, const TagSequence &rhs);
}
#endif // APTOS_TAGSEQUENCE_H
