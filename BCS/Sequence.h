//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_SEQUENCE_H
#define APTOS_SEQUENCE_H
#include "BCSTypes.h"

class Sequence : public ISerializable {
private:
    std::vector<ISerializable*> values;

public:
    Sequence(const std::vector<ISerializable*>& values);

    int Length() const;

    void* GetValue();

    void Serialize(Serialization& serializer);

    static Sequence* Deserialize(Deserialization& deserializer);

    bool Equals(const Sequence& other) const;

    std::string ToString() const;

    size_t GetHashCode() const;
};

#endif //APTOS_SEQUENCE_H
