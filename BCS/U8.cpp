//
// Created by Anh NPH on 21/09/2023.
//

#include "U8.h"
#include "Serialization.h"
#include "Deserialization.h"
#include <sstream>

U8::U8(uint8_t value) {
    this->value = value;
}

void U8::Serialize(Serialization& serializer) {
    serializer.SerializeU8(this->value);
}

U8* U8::Deserialize(Deserialization& deserializer) {
    uint8_t value = deserializer.DeserializeU8();
    return new U8(value);
}

TypeTag U8::Variant() {
    return TypeTag::U8;
}

void* U8::GetValue() {
    return &this->value;
}

bool U8::Equals(const U8& other) const {
    return this->value == other.value;
}

std::string U8::ToString() const {
    std::ostringstream oss;
    oss << static_cast<int>(this->value);
    return oss.str();
}

size_t U8::GetHashCode() const {
    // Simple hash code calculation, more sophisticated method might be needed
    return std::hash<uint8_t>{}(this->value);
}
