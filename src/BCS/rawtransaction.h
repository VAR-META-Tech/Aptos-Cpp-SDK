#ifndef RAWTRANSACTION_H
#define RAWTRANSACTION_H

#include <string>
#include <vector>
#include "../Accounts/AccountAddress.h"
#include "TransactionPayload.h"
#include "../Accounts/PrivateKey.h"

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a raw transaction.
    /// </summary>
    class RawTransaction : public ISerializable
    {
    public:
        explicit RawTransaction(const Accounts::AccountAddress &sender, int sequenceNumber,
                       const TransactionPayload &payload, int maxGasAmount,
                       int gasUnitPrice, uint64_t expirationTimestampsSecs, int chainId);
        std::vector<uint8_t> Prehash();
        std::vector<uint8_t> Keyed();
        Accounts::Signature Sign(Accounts::PrivateKey key);
        bool Verify(Accounts::PublicKey key, const Accounts::Signature &signature);
        void Serialize(Serialization &serializer) const override;
        static std::shared_ptr<ISerializable> Deserialize(Deserialization &deserializer);
        std::string ToString() const override;
        bool Equals(const RawTransaction &other) const;

    private:
        Accounts::AccountAddress sender;
        int sequenceNumber;
        TransactionPayload payload;
        int maxGasAmount;
        int gasUnitPrice;
        uint64_t expirationTimestampsSecs;
        int chainId;
    };

    bool operator==(const RawTransaction &lhs, const RawTransaction &rhs);
}
#endif // RAWTRANSACTION_H
