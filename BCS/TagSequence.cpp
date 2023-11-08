//
// Created by Anh NPH on 21/09/2023.
//

#include "TagSequence.h"
#include <vector>
#include "Serialization.h"
#include "Deserialization.h"
#include <sstream>

TagSequence::TagSequence(const std::vector<std::shared_ptr<ISerializableTag> > &serializableTags) {
    this->serializableTags = serializableTags;
}

void TagSequence::Serialize(Serialization& serializer) const {
    serializer.SerializeU32AsUleb128(this->serializableTags.size());
    for (auto element : this->serializableTags) {
        element->Serialize(serializer);
    }
}

std::shared_ptr<ISerializable> TagSequence::Deserialize(Deserialization& deserializer) {
    int length = deserializer.DeserializeUleb128();

    std::vector<std::shared_ptr<ISerializableTag>> values;

    while (values.size() < length) {
        auto tag = ISerializableTag::Deserialize(deserializer);
        values.push_back(std::dynamic_pointer_cast<ISerializableTag>(tag));
    }

    return std::make_shared<TagSequence>(values);
}

std::vector<std::shared_ptr<ISerializableTag> > TagSequence::GetValue() const {
    return serializableTags;
}

bool TagSequence::Equals(const TagSequence& other) const {
    // Simple equality check, more sophisticated method might be needed
    return this->serializableTags == other.serializableTags;
}

std::string TagSequence::ToString() const {
    std::ostringstream oss;
    for (const auto& tag : this->serializableTags) {
        oss << tag->ToString();
    }
    return oss.str();
}

size_t TagSequence::GetHashCode() const {
    // Simple hash code calculation, more sophisticated method might be needed
//    return std::hash<std::vector<ISerializable*>>{}(this->serializableTags);
}

bool operator==(const TagSequence &lhs, const TagSequence &rhs) {
    return lhs.Equals(rhs);
}
