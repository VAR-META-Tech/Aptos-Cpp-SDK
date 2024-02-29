//
// Created by Anh NPH on 21/09/2023.
//

#include "Bool.h"
#include "Serialization.h"
#include "Deserialization.h"

namespace Aptos::BCS
{
    Bool::Bool(bool value): value(value)
    {
    }

    void Bool::Serialize(Serialization &serializer) const
    {
        serializer.SerializeBool(this->value);
    }

    std::shared_ptr<Bool> Bool::Deserialize(Deserialization &deserializer)
    {
        bool value = deserializer.DeserializeBool();
        return std::make_shared<Bool>(value);
    }

    TypeTag Bool::Variant() const
    {
        return TypeTag::BOOL;
    }

    bool Bool::GetValue() const
    {
        return value;
    }

    bool Bool::Equals(const Bool &other) const
    {
        return this->value == other.value;
    }

    std::string Bool::ToString() const
    {
        return this->value ? "true" : "false";
    }

    size_t Bool::GetHashCode() const
    {
        // Simple hash code calculation, more sophisticated method might be needed
        return std::hash<bool>{}(this->value);
    }

    bool operator==(const Bool &lhs, const Bool &rhs)
    {
        return lhs.Equals(rhs);
    }
}
