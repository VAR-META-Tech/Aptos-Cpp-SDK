//
// Created by Anh NPH on 21/09/2023.
//

#include "Sequence.h"
#include <sstream>
#include <functional>
#include "Bytes.h"
#include "Serialization.h"
#include "Deserialization.h"

Sequence::Sequence(const std::vector<std::shared_ptr<ISerializable> > &values) {
    this->values = values;
}

int Sequence::Length() const {
    return this->values.size();
}

std::vector<std::shared_ptr<ISerializable> > Sequence::GetValue() const {
    return values;
}

void Sequence::Serialize(Serialization& serializer) const {
    serializer.SerializeU32AsUleb128(this->values.size());
    for (const auto& element : this->values) {
        if (std::dynamic_pointer_cast<Sequence>(element)) {
            Serialization seqSerializer;
            auto seq = std::dynamic_pointer_cast<Sequence>(element);
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

std::shared_ptr<ISerializable> Sequence::Deserialize(Deserialization& deserializer) {
    int length = deserializer.DeserializeUleb128();
    std::vector<std::shared_ptr<ISerializable>> values;

    while (values.size() < length) {
        values.push_back(std::make_shared<Bytes>(deserializer.ToBytes()));
    }

    return std::make_shared<Sequence>(values);
}

bool Sequence::Equals(const Sequence& other) const {
    return std::equal(this->values.begin(), this->values.end(), other.values.begin(), other.values.end());
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
