#include "Rawtransaction.h"
#include <stdexcept>
#include <cryptopp/sha3.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include "../HDWallet/Utils/Utils.h"

using namespace Aptos::Accounts;
namespace Aptos::BCS
{
    RawTransaction::RawTransaction(const AccountAddress &sender, int sequenceNumber, const TransactionPayload &payload, int maxGasAmount, int gasUnitPrice, uint64_t expirationTimestampsSecs, int chainId)
        : sender(sender), sequenceNumber(sequenceNumber), payload(payload),
          maxGasAmount(maxGasAmount), gasUnitPrice(gasUnitPrice),
          expirationTimestampsSecs(expirationTimestampsSecs), chainId(chainId)
    {
    }

    std::vector<uint8_t> RawTransaction::Prehash()
    {
        CryptoPP::SHA3_256 sha3;
        std::string input = "APTOS::RawTransaction";
        auto inputS = Utils::StringToSecByteBlock(input.data());
        sha3.Update(inputS, inputS.size());
        CryptoPP::SecByteBlock result(Utils::Ed25519PublicKeySizeInBytes); // 256 bits = 32 bytes
        sha3.Final(result);

        return Utils::SecBlockToByteVector(result);
    }

    std::vector<uint8_t> RawTransaction::Keyed()
    {
        Serialization ser;
        this->Serialize(ser);

        std::vector<uint8_t> prehash = this->Prehash();
        std::vector<uint8_t> outputBytes = ser.GetBytes();

        std::vector<uint8_t> res(prehash.size() + outputBytes.size());
        std::ranges::copy(prehash.begin(), prehash.end(), res.begin());
        std::ranges::copy(outputBytes.begin(), outputBytes.end(), res.begin() + prehash.size());

        return res;
    }

    Ed25519Signature RawTransaction::Sign(PrivateKey key)
    {
        return key.Sign(Utils::ByteVectorToSecBlock(this->Keyed()));
    }

    bool RawTransaction::Verify(PublicKey key, const Ed25519Signature &signature)
    {
        return key.Verify(Utils::ByteVectorToSecBlock(Keyed()), signature);
    }

    void RawTransaction::Serialize(Serialization &serializer) const
    {
        this->sender.Serialize(serializer);
        serializer.SerializeU64(static_cast<uint64_t>(this->sequenceNumber));
        this->payload.Serialize(serializer);
        serializer.SerializeU64(static_cast<uint64_t>(this->maxGasAmount));
        serializer.SerializeU64(static_cast<uint64_t>(this->gasUnitPrice));
        serializer.SerializeU64(static_cast<uint64_t>(this->expirationTimestampsSecs));
        serializer.SerializeU8(static_cast<uint8_t>(this->chainId));
    }

    std::shared_ptr<RawTransaction> RawTransaction::Deserialize(Deserialization &deserializer)
    {
        auto sender = std::dynamic_pointer_cast<AccountAddress>(AccountAddress::Deserialize(deserializer));
        auto sequenceNumber = deserializer.DeserializeU64();
        auto payload = std::dynamic_pointer_cast<TransactionPayload>(TransactionPayload::Deserialize(deserializer));
        auto maxGasAmount = deserializer.DeserializeU64();
        auto gasUnitPrice = deserializer.DeserializeU64();
        auto expirationTimestampsSecs = deserializer.DeserializeU64();
        auto chainId = deserializer.DeserializeU8();

        return std::make_shared<RawTransaction>(
            *sender,
            static_cast<int>(sequenceNumber),
            *payload,
            static_cast<int>(maxGasAmount),
            static_cast<int>(gasUnitPrice),
            static_cast<int>(expirationTimestampsSecs),
            static_cast<int>(chainId));
    }

    std::string RawTransaction::ToString() const
    {
        return "RawTransaction";
    }

    bool RawTransaction::Equals(const RawTransaction &other) const
    {
        return (
            this->sender == other.sender &&
            this->sequenceNumber == other.sequenceNumber &&
            this->payload == other.payload &&
            this->maxGasAmount == other.maxGasAmount &&
            this->gasUnitPrice == other.gasUnitPrice &&
            this->expirationTimestampsSecs == other.expirationTimestampsSecs &&
            this->chainId == other.chainId);
    }

    bool operator==(const RawTransaction &lhs, const RawTransaction &rhs)
    {
        return lhs.Equals(rhs);
    }
}