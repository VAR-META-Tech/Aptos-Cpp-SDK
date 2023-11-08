#ifndef RAWTRANSACTION_H
#define RAWTRANSACTION_H

#include <string>
#include <vector>
#include "../Accounts/AccountAddress.h"
#include "TransactionPayload.h"
#include "../Accounts/PrivateKey.h"

class RawTransaction : public ISerializable {
public:
    RawTransaction(const AccountAddress& sender, int sequenceNumber,
                   const TransactionPayload& payload, int maxGasAmount,
                   int gasUnitPrice, uint64_t expirationTimestampsSecs, int chainId);
    std::vector<uint8_t> Prehash();
    std::vector<uint8_t> Keyed();
    Signature Sign(PrivateKey key);
    bool Verify(PublicKey key, const Signature& signature);
    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    std::string ToString() const override;
    bool Equals(const RawTransaction& other) const;
private:
    AccountAddress sender;
    int sequenceNumber;
    TransactionPayload payload;
    int maxGasAmount;
    int gasUnitPrice;
    uint64_t expirationTimestampsSecs;
    int chainId;
};

bool operator==(const RawTransaction &lhs, const RawTransaction &rhs);

#endif // RAWTRANSACTION_H
