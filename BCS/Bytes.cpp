//
// Created by Anh NPH on 21/09/2023.
//

#include "Bytes.h"

#include "Bytes.h"
#include <algorithm>
#include <sstream>
#include <functional>
#include "Serialization.h"
#include "Deserialization.h"

Bytes::Bytes(const std::vector<uint8_t>& values) {
    this->values = values;
}

void Bytes::Serialize(Serialization& serializer) {
    serializer.SerializeBytes(this->values);
}

Bytes* Bytes::Deserialize(Deserialization& deserializer) {
    std::vector<uint8_t> values = deserializer.ToBytes();
    return new Bytes(values);
}

std::vector<uint8_t> Bytes::getValue() {
    return this->values;
}

bool Bytes::Equals(const Bytes& other) const {
    return std::equal(this->values.begin(), this->values.end(), other.values.begin());
}

std::string Bytes::ToString() const {
    std::ostringstream oss;
    for (const auto& value : values)
        oss << static_cast<int>(value);
    return oss.str();
}

size_t Bytes::GetHashCode() const {
    // Simple hash code calculation, more sophisticated method might be needed
    return std::hash<std::string>{}(this->ToString());
}