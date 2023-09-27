//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_BOOL_H
#define APTOS_BOOL_H
#include "BCSTypes.h"

class Bool : public ISerializableTag {
private:
    bool value;

public:
    Bool(bool value);

    void Serialize(Serialization& serializer);

    static Bool* Deserialize(Deserialization& deserializer);

    TypeTag Variant();

    virtual void* GetValue();

    bool Equals(const Bool& other) const;

    std::string ToString() const;

    size_t GetHashCode() const;
};


#endif //APTOS_BOOL_H
