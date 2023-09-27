//
// Created by Anh NPH on 21/09/2023.
//

#include "TagSequence.h"
#include <vector>
#include "Serialization.h"
#include "Deserialization.h"
#include <sstream>

TagSequence::TagSequence(const std::vector<ISerializable*>& serializableTags) {
    this->serializableTags = serializableTags;
}

void TagSequence::Serialize(Serialization& serializer) {
    serializer.SerializeU32AsUleb128(this->serializableTags.size());
    for (ISerializable* element : this->serializableTags) {
        element->Serialize(serializer);
    }
}

TagSequence* TagSequence::Deserialize(Deserialization& deserializer) {
    int length = deserializer.DeserializeUleb128();

    std::vector<ISerializable*> values;

    while (values.size() < length) {
        ISerializable* tag = ISerializable::Deserialize(deserializer);
        values.push_back(tag);
    }

    return new TagSequence(values);
}

void* TagSequence::GetValue() {
    return &this->serializableTags; // return address of value
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