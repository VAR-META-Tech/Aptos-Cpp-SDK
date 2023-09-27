//
// Created by Anh NPH on 21/09/2023.
//

#include "Sequence.h"
#include <sstream>
#include <functional>
#include "Serialization.h"
#include "Deserialization.h"

Sequence::Sequence(const std::vector<ISerializable*>& values) {
    this->values = values;
}

int Sequence::Length() const {
    return this->values.size();
}

void* Sequence::GetValue() {
    return &this->values; // return address of value
}

void Sequence::Serialize(Serialization& serializer) {
    serializer.SerializeU32AsUleb128(this->values.size());
    for (ISerializable* element : this->values) {
        if (dynamic_cast<Sequence*>(element)) {
            Serialization seqSerializer;
            Sequence* seq = dynamic_cast<Sequence*>(element);
            seq->Serialize(seqSerializer);

            std::vector<uint8_t> elementsBytes = seqSerializer.GetBytes();
            int sequenceLen = elementsBytes.size();
            serializer.SerializeU32AsUleb128(sequenceLen);
            serializer.SerializeFixedBytes(elementsBytes);
        }
        else {
            Serialization s;
            element->Serialize(s);
            std::vector<uint8_t> b = s.GetBytes();
            serializer.SerializeBytes(b);
        }
    }
}

Sequence* Sequence::Deserialize(Deserialization& deserializer) {
    int length = deserializer.DeserializeUleb128();

    std::vector<ISerializable*> values;

    while (values.size() < length) {
        ISerializable* value = ISerializable::Deserialize(deserializer);
        values.push_back(value);
    }

    return new Sequence(values);
}

bool Sequence::Equals(const Sequence& other) const {
    // Simple equality check, more sophisticated method might be needed
    return this->values == other.values;
}

std::string Sequence::ToString() const {
    std::ostringstream oss;
    for (const auto& value : this->values) {
        oss << value->ToString();
    }
    return oss.str();
}

size_t Sequence::GetHashCode() const {
    // Simple hash code calculation, more sophisticated method might be needed
//    return std::hash<std::vector<ISerializable*>>{}(this->values);
}