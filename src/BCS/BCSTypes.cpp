//
// Created by Anh NPH on 21/09/2023.
//

#include "BCSTypes.h"
#include "Deserialization.h"
#include <algorithm>
#include "Bool.h"
#include "U8.h"
#include "U16.h"
#include "U32.h"
#include "U64.h"
#include "U128.h"
#include "U256.h"
#include "../Accounts/AccountAddress.h"
#include "StructTag.h"

namespace Aptos::BCS
{
    std::shared_ptr<ISerializableTag> ISerializableTag::DeserializeTag(Deserialization &deserializer)
    {
        auto variant = static_cast<TypeTag>(deserializer.DeserializeUleb128());

        switch (variant)
        {
        case TypeTag::BOOL:
            return Bool::Deserialize(deserializer);
        case TypeTag::U8:
            return U8::Deserialize(deserializer);
        case TypeTag::U16:
            return U16::Deserialize(deserializer);
        case TypeTag::U32:
            return U32::Deserialize(deserializer);
        case TypeTag::U64:
            return U64::Deserialize(deserializer);
        case TypeTag::U128:
            return U128::Deserialize(deserializer);
        case TypeTag::U256:
            return U256::Deserialize(deserializer);
        case TypeTag::ACCOUNT_ADDRESS:
            return Accounts::AccountAddress::Deserialize(deserializer);
        case TypeTag::STRUCT:
            return StructTag::Deserialize(deserializer);
        default:
            throw std::logic_error("The method or operation is not implemented.");
        }
    }

    std::string ISerializableTag::ToString() const
    {
        std::string typeTagStr;
        switch (Variant())
        {
        case TypeTag::BOOL:
            typeTagStr = "BOOL";
            break;
        case TypeTag::U8:
            typeTagStr = "U8";
            break;
        case TypeTag::U64:
            typeTagStr = "U64";
            break;
        case TypeTag::U128:
            typeTagStr = "U128";
            break;
        case TypeTag::ACCOUNT_ADDRESS:
            typeTagStr = "ACCOUNT_ADDRESS";
            break;
        case TypeTag::SIGNER:
            typeTagStr = "SIGNER";
            break;
        case TypeTag::VECTOR:
            typeTagStr = "VECTOR";
            break;
        case TypeTag::STRUCT:
            typeTagStr = "STRUCT";
            break;
        case TypeTag::U16:
            typeTagStr = "U16";
            break;
        case TypeTag::U32:
            typeTagStr = "U32";
            break;
        case TypeTag::U256:
            typeTagStr = "U256";
            break;
        default:
            typeTagStr = "UNKNOWN";
        }
        return "ISerializableTag (" + typeTagStr + ")";
    }

    std::shared_ptr<ISerializable> ISerializable::Deserialize(Deserialization &deserializer)
    {
        throw std::logic_error("The method or operation is not implemented.");
        return nullptr;
    }

    std::size_t ISerializable::GetHashCode() const
    {
        std::size_t seed = 0;
        return seed;
    }
}