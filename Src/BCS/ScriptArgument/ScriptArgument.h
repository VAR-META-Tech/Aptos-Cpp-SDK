#ifndef SCRIPTARGUMENT_H
#define SCRIPTARGUMENT_H
#include <iostream>
#include "../BCSTypes.h"

namespace Aptos::BCS
{
    enum class ScriptArgumentTypeTag
    {
        U8,
        U64,
        U128,
        ACCOUNT_ADDRESS,
        U8_VECTOR,
        BOOL,
        U16,
        U32,
        U256
    };

    /// <summary>
    /// Representation of a script argument.
    /// </summary>
    class APTOS_API ScriptArgument : public ISerializable
    {
    public:
        explicit ScriptArgument(ScriptArgumentTypeTag variant, std::shared_ptr<ISerializableTag> value);
        void Serialize(Serialization &serializer) const override;
        static std::shared_ptr<ScriptArgument> Deserialize(Deserialization &deserializer);
        bool Equals(const ScriptArgument &other);
        std::string ToString() const override;
        size_t GetHashCode() const override;

    private:
        ScriptArgumentTypeTag variant;
        std::shared_ptr<ISerializableTag> value;
    };
}
#endif // SCRIPTARGUMENT_H
