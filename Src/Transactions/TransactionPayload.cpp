#include "TransactionPayload.h"
#include "BCS/BCS.h"
#include "BCS/EntryFunctions/EntryFunction.h"

namespace Aptos::BCS
{
    TransactionPayload::TransactionPayload(const std::shared_ptr<ISerializable> &payload)
    {
        if (std::dynamic_pointer_cast<Script>(payload))
        {
            this->variant = TransactionPayloadTypeTag::SCRIPT;
        }
        else if (std::dynamic_pointer_cast<EntryFunction>(payload))
        {
            this->variant = TransactionPayloadTypeTag::SCRIPT_FUNCTION;
        }
        else
        {
            throw std::invalid_argument("Invalid type");
        }

        this->value = payload;
    }

    TransactionPayloadTypeTag TransactionPayload::Variant() const
    {
        return this->variant;
    }

    void TransactionPayload::Serialize(Serialization &serializer) const
    {
        serializer.SerializeU32AsUleb128(static_cast<uint32_t>(this->Variant()));
        this->value->Serialize(serializer);
    }

    std::shared_ptr<TransactionPayload> TransactionPayload::Deserialize(Deserialization &deserializer)
    {
        auto variant = static_cast<TransactionPayloadTypeTag>(deserializer.DeserializeUleb128());
        std::shared_ptr<ISerializable> payload = nullptr;
        if (variant == TransactionPayloadTypeTag::SCRIPT)
        {
            payload = Script::Deserialize(deserializer);
        }
        else if (variant == TransactionPayloadTypeTag::SCRIPT_FUNCTION)
        {
            payload = EntryFunction::Deserialize(deserializer);
        }
        else
        {
            throw std::invalid_argument("Invalid type " + std::to_string(static_cast<int>(variant)));
        }

        return std::make_shared<TransactionPayload>(payload);
    }

    bool TransactionPayload::Equals(const TransactionPayload &other) const
    {
        bool variantComparison = this->variant == other.variant;
        bool txnPayload = false;
        if (variantComparison)
        {
            auto script = std::dynamic_pointer_cast<Script>(value);
            if (script)
            {
                txnPayload = script->Equals(*(std::dynamic_pointer_cast<Script>(other.value)));
            }
            else
            {
                auto func = std::dynamic_pointer_cast<EntryFunction>(value);
                if (func)
                {
                    txnPayload = func->Equals(*(std::dynamic_pointer_cast<EntryFunction>(other.value)));
                }
                else
                {
                    txnPayload = false;
                }
            }
        }
        else
        {
            return false;
        }
        return variantComparison && txnPayload;
    }

    std::string TransactionPayload::ToString() const
    {
        return this->value->ToString();
    }

    size_t TransactionPayload::GetHashCode() const
    {
        size_t hash = 17;
        hash = hash * 23 + this->value->GetHashCode();
        hash = hash * 23 + static_cast<size_t>(this->variant);
        return hash;
    }

    bool operator==(const TransactionPayload &lhs, const TransactionPayload &rhs)
    {
        return lhs.Equals(rhs);
    }
}