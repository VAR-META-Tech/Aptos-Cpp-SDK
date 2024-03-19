#include "ScriptArgument.h"
#include <stdexcept>
#include "../Bool.h"
#include "../Serialization.h"
#include "../../Accounts/AccountAddress.h"
#include "../U128.h"
#include "../U16.h"
#include "../U256.h"
#include "../U32.h"
#include "../U64.h"
#include "../U8.h"

using namespace Aptos::Accounts;
namespace Aptos::BCS
{
    ScriptArgument::ScriptArgument(ScriptArgumentTypeTag variant, std::shared_ptr<ISerializableTag> value)
    {
        if (variant < ScriptArgumentTypeTag::U8 || variant > ScriptArgumentTypeTag::U256)
        {
            throw std::invalid_argument("Invalid variant");
        }

        this->variant = variant;
        this->value = value;
    }

    void ScriptArgument::Serialize(Serialization &serializer) const
    {
        serializer.SerializeU8(static_cast<uint8_t>(this->variant));
        if (this->variant == ScriptArgumentTypeTag::U8)
        {
            serializer.SerializeU8(std::dynamic_pointer_cast<U8>(this->value)->GetValue());
        }
        else if (this->variant == ScriptArgumentTypeTag::U16)
        {
            serializer.SerializeU16(std::dynamic_pointer_cast<U16>(this->value)->GetValue());
        }
        else if (this->variant == ScriptArgumentTypeTag::U32)
        {
            serializer.SerializeU32(std::dynamic_pointer_cast<U32>(this->value)->GetValue());
        }
        else if (this->variant == ScriptArgumentTypeTag::U64)
        {
            serializer.SerializeU64(std::dynamic_pointer_cast<U64>(this->value)->GetValue());
        }
        else if (this->variant == ScriptArgumentTypeTag::U128)
        {
            serializer.SerializeU128(std::dynamic_pointer_cast<U128>(this->value)->GetValue());
        }
        else if (this->variant == ScriptArgumentTypeTag::U256)
        {
            serializer.SerializeU256(std::dynamic_pointer_cast<U256>(this->value)->GetValue());
        }
        else if (this->variant == ScriptArgumentTypeTag::ACCOUNT_ADDRESS)
        {
            serializer.Serialize(*std::dynamic_pointer_cast<AccountAddress>(this->value));
        }
        else if (this->variant == ScriptArgumentTypeTag::BOOL)
        {
            serializer.SerializeBool(std::dynamic_pointer_cast<Bool>(this->value)->GetValue());
        }
        else
        {
            throw std::invalid_argument("Invalid ScriptArgument variant " + std::to_string(static_cast<uint8_t>(this->variant)));
        }
    }

    std::shared_ptr<ScriptArgument> ScriptArgument::Deserialize(Deserialization &deserializer)
    {
        auto variant = static_cast<ScriptArgumentTypeTag>(deserializer.DeserializeU8());
        std::shared_ptr<ISerializableTag> value;

        if (variant == ScriptArgumentTypeTag::U8)
        {
            value = std::make_shared<U8>(deserializer.DeserializeU8());
        }
        else if (variant == ScriptArgumentTypeTag::U16)
        {
            value = std::make_shared<U16>(deserializer.DeserializeU16());
        }
        else if (variant == ScriptArgumentTypeTag::U32)
        {
            value = std::make_shared<U32>(deserializer.DeserializeU32());
        }
        else if (variant == ScriptArgumentTypeTag::U64)
        {
            value = std::make_shared<U64>(deserializer.DeserializeU64());
        }
        else if (variant == ScriptArgumentTypeTag::U128)
        {
            value = std::make_shared<U128>(deserializer.DeserializeU128());
        }
        else if (variant == ScriptArgumentTypeTag::U256)
        {
            value = std::make_shared<U256>(deserializer.DeserializeU256());
        }
        else if (variant == ScriptArgumentTypeTag::ACCOUNT_ADDRESS)
        {
            value = AccountAddress::Deserialize(deserializer);
        }
        else if (variant == ScriptArgumentTypeTag::BOOL)
        {
            value = std::make_shared<Bool>(deserializer.DeserializeBool());
        }
        else
        {
            throw std::invalid_argument("Invalid variant");
        }

        return std::make_shared<ScriptArgument>(variant, value);
    }

    bool ScriptArgument::Equals(const ScriptArgument &other)
    {
        if (variant == other.variant)
        {
            if (variant == ScriptArgumentTypeTag::U8)
            {
                return std::dynamic_pointer_cast<U8>(value)->Equals(*std::dynamic_pointer_cast<U8>(other.value));
            }
            else if (this->variant == ScriptArgumentTypeTag::U16)
            {
                return std::dynamic_pointer_cast<U16>(value)->Equals(*std::dynamic_pointer_cast<U16>(other.value));
            }
            else if (this->variant == ScriptArgumentTypeTag::U32)
            {
                return std::dynamic_pointer_cast<U32>(value)->Equals(*std::dynamic_pointer_cast<U32>(other.value));
            }
            else if (this->variant == ScriptArgumentTypeTag::U64)
            {
                return std::dynamic_pointer_cast<U64>(value)->Equals(*std::dynamic_pointer_cast<U64>(other.value));
            }
            else if (this->variant == ScriptArgumentTypeTag::U128)
            {
                return std::dynamic_pointer_cast<U128>(value)->Equals(*std::dynamic_pointer_cast<U128>(other.value));
            }
            else if (this->variant == ScriptArgumentTypeTag::U256)
            {
                return std::dynamic_pointer_cast<U256>(value)->Equals(*std::dynamic_pointer_cast<U256>(other.value));
            }
            else if (this->variant == ScriptArgumentTypeTag::ACCOUNT_ADDRESS)
            {
                return *std::dynamic_pointer_cast<AccountAddress>(value) == (*std::dynamic_pointer_cast<AccountAddress>(other.value));
            }
            else if (this->variant == ScriptArgumentTypeTag::BOOL)
            {
                return std::dynamic_pointer_cast<Bool>(value)->Equals(*std::dynamic_pointer_cast<Bool>(other.value));
            }
            else
            {
                return false;
            }
        }
        return false;
    }

    std::string ScriptArgument::ToString() const
    {
        return "[" + std::to_string(static_cast<uint8_t>(this->variant)) + "] " + this->value->ToString();
    }

    size_t ScriptArgument::GetHashCode() const
    {
        size_t hash = 17;
        hash = hash * 23 + static_cast<size_t>(this->variant);
        hash = hash * 23 + this->value->GetHashCode();
        return hash;
    }
}