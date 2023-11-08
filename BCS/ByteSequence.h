//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_BYTESEQUENCE_H
#define APTOS_BYTESEQUENCE_H
#include "BCSTypes.h"

class BytesSequence : public ISerializable {
private:
    std::vector<std::vector<uint8_t>> values;

public:
    BytesSequence(const std::vector<std::vector<uint8_t>>& values);

    void Serialize(Serialization& serializer) const override;

    static BytesSequence* Deserialize(Deserialization& deserializer);

    std::vector<std::vector<uint8_t>> GetValue() const;

    bool Equals(const BytesSequence& other) const;

    std::string ToString() const override;

    size_t GetHashCode() const;
};


#endif //APTOS_BYTESEQUENCE_H
