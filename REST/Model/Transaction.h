#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "TransactionRequest.h"

namespace AptosRESTModel {

class TransactionEvent {
public:
    class DataObject {
    public:
        static DataObject FromJson(const nlohmann::json& j);
        nlohmann::json ToJson() const;
    private:
        std::string Index;
        std::string Token;
        std::string From;
        std::string To;
        std::string Object;
    };

    class GUIDAddress {
    public:
        static GUIDAddress FromJson(const nlohmann::json& j);
        nlohmann::json ToJson() const;
    private:
        std::string CreationNumber;
        std::string AccountAddress;
    };

    static TransactionEvent FromJson(const nlohmann::json& j);
    nlohmann::json ToJson() const;
private:
    GUIDAddress GUID;
    std::string SequenceNumber;
    std::string Type;
    DataObject Data;
};

class Change {
public:
    nlohmann::json ToJson() const;
    static Change FromJson(const nlohmann::json& changeJson);
protected:
    std::string Type;
};

class ChangeWriteResource : public Change {
public:
    nlohmann::json ToJson() const;
    static ChangeWriteResource FromJson(const nlohmann::json& changeWriteResourceJson);
protected:
    std::string Address;
    std::string StateKeyHash;
};

class ChangeWriteResourceAptosCoin : public ChangeWriteResource {};

class ChangeWriteResourceAccount : public ChangeWriteResource {
public:
    nlohmann::json ToJson() const;
    static ChangeWriteResourceAccount FromJson(const nlohmann::json& changeWriteResourceAccountJson);
private:
    std::string AuthenticationKey;
    std::string GuidCreationNum;
    std::string SequenceNumber;
};

class ChangeWriteResourceWriteTableItem : public Change {
public:
    nlohmann::json ToJson() const;
    static ChangeWriteResourceWriteTableItem FromJson(const nlohmann::json& changeWriteResourceWriteTableItemJson);
private:
    std::string StateKeyHash;
    std::string Handle;
    std::string Key;
    std::string Value;
    std::string Data;
};

/// <summary>
/// Represent a Transaction response
/// NOTE: A TransactionRequest is a subset of a Transaction
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/Transaction
/// </summary>
class Transaction : public TransactionRequest {
public:
    Transaction(const TransactionRequest& transactionRequest);
    Transaction();
    static Transaction FromJson(const nlohmann::json& j);
    nlohmann::json ToJson() const;

    std::string getType() const;
    void setType(const std::string &newType);

    bool getSuccess() const;
    void setSuccess(bool newSuccess);

    std::string getHash() const;

private:
    std::string Type;
    std::string Version;
    std::string Hash;
    std::string StateChangeHash;
    std::string EventRootHash;
    std::string StateCheckpointHash;
    std::string GasUsed;
    bool Success;
    std::string VmStatus;
    std::string AccumulatorRootHash;
    std::string Timestamp;
    std::vector<Change> Changes;
    std::vector<TransactionEvent> Events;
};

}
#endif // TRANSACTION_H
