#ifndef TRANSACTIONREQUEST_H
#define TRANSACTIONREQUEST_H

#include <nlohmann/json.hpp>
#include "TransactionPayload.h"
#include "../../BCS/EntryFunctions/EntryFunction.h"

namespace AptosRESTModel {

class APTOS_API SignatureData {
public:
    nlohmann::json ToJson() const;
    static SignatureData FromJson(const nlohmann::json& signatureJson);
    std::string getType() const;
    std::string getPublicKey() const;
    std::string getSignature() const;
    void setType(const std::string &newType);
    void setPublicKey(const std::string &newPublicKey);
    void setSignature(const std::string &newSignature);

private:
    std::string Type;
    std::string PublicKey;
    std::string Signature;
};

/// <summary>
/// Represents a Transaction Request, a subset of Transaction
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/Transaction
/// </summary>
class APTOS_API TransactionRequest {
public:
    virtual nlohmann::json ToJson() const;
    static TransactionRequest FromJson(const nlohmann::json& requestJson);
    std::string getSender() const;
    void setSender(const std::string &newSender);

    std::string getSequenceNumber() const;
    void setSequenceNumber(const std::string &newSequenceNumber);

    std::string getMaxGasAmount() const;
    void setMaxGasAmount(const std::string &newMaxGasAmount);

    std::string getGasUnitPrice() const;
    void setGasUnitPrice(const std::string &newGasUnitPrice);

    std::string getExpirationTimestampSecs() const;
    void setExpirationTimestampSecs(const std::string &newExpirationTimestampSecs);

    SignatureData getSignature() const;
    void setSignature(const SignatureData &newSignature);

    Aptos::BCS::EntryFunction getEntryFunction() const;
    void setEntryFunction(const Aptos::BCS::EntryFunction &newEntryFunction);

private:
    std::string Sender;
    std::string SequenceNumber;
    std::string MaxGasAmount;
    std::string GasUnitPrice;
    std::string ExpirationTimestampSecs;
    TransactionPayload Payload;
    Aptos::BCS::EntryFunction EntryFunction;
    SignatureData Signature;
};

}
#endif // TRANSACTIONREQUEST_H
