#ifndef TRANSACTIONPAYLOAD_H
#define TRANSACTIONPAYLOAD_H
#include "BCS/BCS.h"

namespace Aptos::BCS
{
    enum class TransactionPayloadTypeTag
    {
        SCRIPT,
        MODULE_BUNDLE,
        SCRIPT_FUNCTION
    };

    /// <summary>
    /// Representation of a transaction's payload.
    /// </summary>
    class APTOS_API TransactionPayload : public ISerializable
    {
    public:
        explicit TransactionPayload(const std::shared_ptr<ISerializable> &payload);
        TransactionPayloadTypeTag Variant() const;
        void Serialize(Serialization &serializer) const override;
        static std::shared_ptr<TransactionPayload> Deserialize(Deserialization &deserializer);
        bool Equals(const TransactionPayload &other) const;
        std::string ToString() const override;
        size_t GetHashCode() const override;

    private:
        std::shared_ptr<ISerializable> value;
        TransactionPayloadTypeTag variant;
    };

    bool APTOS_API operator==(const TransactionPayload &lhs, const TransactionPayload &rhs);
}
#endif // TRANSACTIONPAYLOAD_H
