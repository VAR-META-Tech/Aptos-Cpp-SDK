#include "ModuleId.h"

using namespace Aptos::Accounts;
namespace Aptos::BCS
{

ModuleId::ModuleId(const AccountAddress &address, const std::string &name)
        : address(address), name(name) {}

    void ModuleId::Serialize(Serialization &serializer) const
    {
        address.Serialize(serializer);
        serializer.SerializeString(name);
    }

    std::shared_ptr<ModuleId> ModuleId::Deserialize(Deserialization &deserializer)
    {
        auto addr = std::dynamic_pointer_cast<AccountAddress>(AccountAddress::Deserialize(deserializer));
        std::string name = deserializer.DeserializeString();
        return std::make_shared<ModuleId>(*addr, name);
    }

    bool ModuleId::Equals(const ModuleId &other) const
    {
        return address == other.address && name == other.name;
    }

    std::string ModuleId::ToString() const
    {
        return address.ToString() + "::" + name;
    }

    ModuleId ModuleId::FromStr(const std::string &moduleId)
    {
        if (moduleId.empty())
        {
            throw std::invalid_argument("ModuleId string is empty.");
        }

        size_t pos = moduleId.find("::");
        if (pos == std::string::npos || pos == 0 || pos == moduleId.size() - 2)
        {
            throw std::invalid_argument("Invalid ModuleId format.");
        }

        std::string addressStr = moduleId.substr(0, pos);
        AccountAddress accountAddress = AccountAddress::FromHex(addressStr);
        std::string name = moduleId.substr(pos + 2);

        return ModuleId(accountAddress, name);
    }

    size_t ModuleId::GetHashCode() const
    {
        return 0;
        //    size_t hash = 17;
        //    hash = hash * 23 + address.GetHashCode();
        //    hash = hash * 23 + std::hash<std::string>{}(name);
        //    return hash;
    }

    bool operator==(const ModuleId &lhs, const ModuleId &rhs)
    {
        return lhs.Equals(rhs);
    }
}