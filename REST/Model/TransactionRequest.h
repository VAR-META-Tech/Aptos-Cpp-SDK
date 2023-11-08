#ifndef TRANSACTIONREQUEST_H
#define TRANSACTIONREQUEST_H

#include <nlohmann/json.hpp>
#include "TransactionPayload.h"

namespace AptosRESTModel {

class SignatureData {
public:
    nlohmann::json ToJson() const;
    static SignatureData FromJson(const nlohmann::json& signatureJson);
    std::string getType() const;
    std::string getPublicKey() const;
    std::string getSignature() const;

private:
    std::string Type;
    std::string PublicKey;
    std::string Signature;
};

/// <summary>
/// Represents a Transaction Request, a subset of Transaction
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/Transaction
/// </summary>
class TransactionRequest {
public:
    nlohmann::json ToJson() const;
    static TransactionRequest FromJson(const nlohmann::json& requestJson);
private:
    std::string Sender;
    std::string SequenceNumber;
    std::string MaxGasAmount;
    std::string GasUnitPrice;
    std::string ExpirationTimestampSecs;
    TransactionPayload Payload;
//    EntryFunction EntryFunction;
    SignatureData Signature;
};

}
#endif // TRANSACTIONREQUEST_H
