#ifndef MODULEID_H
#define MODULEID_H


#include <string>
#include <stdexcept>
#include "../Accounts/AccountAddress.h"

class ModuleId : public ISerializable {
public:
    ModuleId(const AccountAddress& address, const std::string& name);
    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    bool Equals(const ModuleId& other) const;
    std::string ToString() const;
    static ModuleId FromStr(const std::string& moduleId);
    size_t GetHashCode() const;
private:
    AccountAddress address;
    std::string name;
};

bool operator==(const ModuleId &lhs, const ModuleId &rhs);

#endif // MODULEID_H
