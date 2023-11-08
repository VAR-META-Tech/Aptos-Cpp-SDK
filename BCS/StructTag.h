#ifndef STRUCTTAG_H
#define STRUCTTAG_H

#include <string>
#include <vector>
#include "BCSTypes.h"
#include "../Accounts/AccountAddress.h"

class StructTag : public ISerializableTag {
public:
    StructTag(AccountAddress address, std::string module, std::string name,
              const std::vector<std::shared_ptr<ISerializableTag> >& typeArgs);

    TypeTag Variant() const override;
    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializableTag> Deserialize(Deserialization& deserializer);
    bool Equals(const StructTag &other) const;
    std::string ToString() const override;
    static StructTag FromStr(const std::string& typeTag);

private:
    AccountAddress address;
    std::string module;
    std::string name;
    std::vector<std::shared_ptr<ISerializableTag>> typeArgs;
};

bool operator==(const StructTag &lhs, const StructTag &rhs);

#endif // STRUCTTAG_H
