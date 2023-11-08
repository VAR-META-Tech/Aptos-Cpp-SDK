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

    void Serialize(Serialization& serializer) const override;

    static std::shared_ptr<ISerializableTag> Deserialize(Deserialization& deserializer);

    TypeTag Variant() const override;

    bool GetValue() const;

    bool Equals(const Bool& other) const;

    std::string ToString() const override;

    size_t GetHashCode() const;
};

bool operator==(const Bool &lhs, const Bool &rhs);

#endif //APTOS_BOOL_H
