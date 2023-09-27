//
// Created by Anh NPH on 21/09/2023.
//

#include "Bool.h"
#include "Serialization.h"
#include "Deserialization.h"


Bool::Bool(bool value) {
    this->value = value;
}

void Bool::Serialize(Serialization& serializer) {
    serializer.SerializeBool(this->value);
}

Bool* Bool::Deserialize(Deserialization& deserializer) {
    bool value = deserializer.DeserializeBool();
    return new Bool(value);
}

TypeTag Bool::Variant() {
    return TypeTag::BOOL;
}

void* Bool::GetValue() {
    return &this->value;
}

bool Bool::Equals(const Bool& other) const {
    return this->value == other.value;
}

std::string Bool::ToString() const {
    return this->value ? "true" : "false";
}

size_t Bool::GetHashCode() const {
    // Simple hash code calculation, more sophisticated method might be needed
    return std::hash<bool>{}(this->value);
}
