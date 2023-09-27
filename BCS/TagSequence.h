//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_TAGSEQUENCE_H
#define APTOS_TAGSEQUENCE_H
#include "BCSTypes.h"

class TagSequence : public ISerializable {
private:
    std::vector<ISerializable*> serializableTags;

public:
    TagSequence(const std::vector<ISerializable*>& serializableTags);

    void Serialize(Serialization& serializer);

    static TagSequence* Deserialize(Deserialization& deserializer);

    void* GetValue();

    bool Equals(const TagSequence& other) const;

    std::string ToString() const;

    size_t GetHashCode() const;
};


#endif //APTOS_TAGSEQUENCE_H
