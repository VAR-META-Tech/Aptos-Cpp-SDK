#include "TransactionPayload.h"
#include "Script.h"
#include "EntryFunction.h"

TransactionPayload::TransactionPayload(const std::shared_ptr<ISerializable> &payload) {
    if (std::dynamic_pointer_cast<Script>(payload)) {
        this->variant = TypeTag::SCRIPT;
    } else if (std::dynamic_pointer_cast<EntryFunction>(payload)) {
        this->variant = TypeTag::ENTRY_FUNCTION;
    } else {
        throw std::invalid_argument("Invalid type");
    }

    this->value = payload;
}

TypeTag TransactionPayload::Variant() const {
    return this->variant;
}

void TransactionPayload::Serialize(Serialization &serializer) const {
    serializer.SerializeU32AsUleb128(static_cast<uint32_t>(this->Variant()));
    this->value->Serialize(serializer);
}

std::shared_ptr<ISerializable> TransactionPayload::Deserialize(Deserialization &deserializer) {
    TypeTag variant = static_cast<TypeTag>(deserializer.DeserializeUleb128());
    std::shared_ptr<ISerializable> payload = nullptr;

    if (variant == TypeTag::SCRIPT) {
        payload = Script::Deserialize(deserializer);
    } else if (variant == TypeTag::ENTRY_FUNCTION) {
        payload = EntryFunction::Deserialize(deserializer);
    } else {
        throw std::invalid_argument("Invalid type");
    }

    return payload;
}

bool TransactionPayload::Equals(const TransactionPayload &other) const {
    bool variantComparison = this->variant == other.variant;
    bool txnPayload = false;
    if (variantComparison) {
        auto script = std::dynamic_pointer_cast<Script>(value);
        if (script) {
            txnPayload = script->Equals(*(std::dynamic_pointer_cast<Script>(other.value)));
        } else {
            auto func = std::dynamic_pointer_cast<EntryFunction>(value);
            if (func) {
                txnPayload = func->Equals(*(std::dynamic_pointer_cast<EntryFunction>(other.value)));
            } else {
                txnPayload = false;
            }
        }
    } else {
        return false;
    }
    return variantComparison && txnPayload;
}

std::string TransactionPayload::ToString() const {
    return this->value->ToString();
}

size_t TransactionPayload::GetHashCode() {
    size_t hash = 17;
    hash = hash * 23 + this->value->GetHashCode();
    hash = hash * 23 + static_cast<size_t>(this->variant);
    return hash;
}

bool operator==(const TransactionPayload &lhs, const TransactionPayload &rhs) {
    return lhs.Equals(rhs);
}
