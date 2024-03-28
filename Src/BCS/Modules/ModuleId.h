#ifndef MODULEID_H
#define MODULEID_H

#include <string>
#include <stdexcept>
#include "../../Accounts/AccountAddress.h"
namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a module ID.
    /// </summary>
    class APTOS_API ModuleId : public ISerializable
    {
    public:
        ModuleId() = default;
        explicit ModuleId(const Accounts::AccountAddress &address, const std::string &name);
        void Serialize(Serialization &serializer) const override;
        static std::shared_ptr<ModuleId> Deserialize(Deserialization &deserializer);
        bool Equals(const ModuleId &other) const;
        std::string ToString() const override;
        static ModuleId FromStr(const std::string &moduleId);
        size_t GetHashCode() const override;

    private:
        Accounts::AccountAddress address;
        std::string name;
    };

    bool APTOS_API operator==(const ModuleId &lhs, const ModuleId &rhs);
}
#endif // MODULEID_H
