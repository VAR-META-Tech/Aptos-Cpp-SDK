#ifndef TRANSACTIONPAYLOAD_H
#define TRANSACTIONPAYLOAD_H

#include "BCSTypes.h"

class TransactionPayload : public ISerializable {
public:
    TransactionPayload(const std::shared_ptr<ISerializable>& payload);
    TypeTag Variant() const;
    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    bool Equals(const TransactionPayload& other) const;
    std::string ToString() const override;
    size_t GetHashCode() override;

private:
    std::shared_ptr<ISerializable> value;
    TypeTag variant;
};

bool operator==(const TransactionPayload &lhs, const TransactionPayload &rhs);

#endif // TRANSACTIONPAYLOAD_H
