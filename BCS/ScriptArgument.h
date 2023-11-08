#ifndef SCRIPTARGUMENT_H
#define SCRIPTARGUMENT_H

#include <iostream>
#include "BCSTypes.h"

class ScriptArgument : public ISerializable {
public:
    ScriptArgument(TypeTag variant, std::shared_ptr<ISerializableTag> value);
    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    bool Equals(const ScriptArgument& other);
    std::string ToString() const override;
    size_t GetHashCode() override;
private:
    TypeTag variant;
    std::shared_ptr<ISerializableTag> value;
};

#endif // SCRIPTARGUMENT_H
