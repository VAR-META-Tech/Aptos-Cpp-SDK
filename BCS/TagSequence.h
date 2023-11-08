//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_TAGSEQUENCE_H
#define APTOS_TAGSEQUENCE_H
#include "BCSTypes.h"

class TagSequence : public ISerializable {
public:
    TagSequence(const std::vector<std::shared_ptr<ISerializableTag>>& serializableTags);
    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    std::vector<std::shared_ptr<ISerializableTag>> GetValue() const;
    bool Equals(const TagSequence& other) const;
    std::string ToString() const override;
    size_t GetHashCode() const;

private:
    std::vector<std::shared_ptr<ISerializableTag>> serializableTags;
};

bool operator==(const TagSequence &lhs, const TagSequence &rhs);

#endif //APTOS_TAGSEQUENCE_H
