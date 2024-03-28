#ifndef RAWTRANSACTION_H
#define RAWTRANSACTION_H

#include <string>
#include <vector>
#include "../Accounts/AccountAddress.h"
#include "TransactionPayload.h"
#include "../Accounts/Ed25519/PrivateKey.h"
using namespace Aptos::Accounts::Ed25519;

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a raw transaction.
    /// </summary>
    class APTOS_API RawTransaction : public ISerializable
    {
    public:
        explicit RawTransaction(const Accounts::AccountAddress &sender, int sequenceNumber,
                                const TransactionPayload &payload, int maxGasAmount,
                                int gasUnitPrice, uint64_t expirationTimestampsSecs, int chainId);
        std::vector<uint8_t> Prehash();
        std::vector<uint8_t> Keyed();
        Accounts::Ed25519Signature Sign(PrivateKey key);
        bool Verify(PublicKey key, const Accounts::Ed25519Signature &signature);
        void Serialize(Serialization &serializer) const override;
        static std::shared_ptr<RawTransaction> Deserialize(Deserialization &deserializer);
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

    bool APTOS_API operator==(const RawTransaction &lhs, const RawTransaction &rhs);
}
#endif // RAWTRANSACTION_H
