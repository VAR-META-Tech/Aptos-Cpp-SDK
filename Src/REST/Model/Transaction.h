#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "TransactionRequest.h"

namespace AptosRESTModel {

class APTOS_API TransactionEvent {
public:
    class APTOS_API DataObject {
    public:
        static DataObject FromJson(const nlohmann::json& j);
        nlohmann::json ToJson() const;
        std::string getToken() const;
        void setToken(const std::string &newToken);

    private:
        std::string Index;
        std::string Token;
        std::string From;
        std::string To;
        std::string Object;
    };

    class APTOS_API GUIDAddress {
    public:
        static GUIDAddress FromJson(const nlohmann::json& j);
        nlohmann::json ToJson() const;
    private:
        std::string CreationNumber;
        std::string AccountAddress;
    };

    static TransactionEvent FromJson(const nlohmann::json& j);
    nlohmann::json ToJson() const;
    std::string getType() const;
    void setType(const std::string &newType);

    std::string getSequenceNumber() const;
    void setSequenceNumber(const std::string &newSequenceNumber);

    DataObject getData() const;
    void setData(const DataObject &newData);

private:
    GUIDAddress GUID;
    std::string SequenceNumber;
    std::string Type;
    DataObject Data;
};

class APTOS_API Change {
public:
    virtual nlohmann::json ToJson() const;
    static Change FromJson(const nlohmann::json& changeJson);
protected:
    std::string Type;
};

class APTOS_API ChangeWriteResource : public Change {
public:
    virtual nlohmann::json ToJson() const;
    static ChangeWriteResource FromJson(const nlohmann::json& changeWriteResourceJson);
protected:
    std::string Address;
    std::string StateKeyHash;
};

class APTOS_API ChangeWriteResourceAptosCoin : public ChangeWriteResource {};

class APTOS_API ChangeWriteResourceAccount : public ChangeWriteResource {
public:
    nlohmann::json ToJson() const;
    static ChangeWriteResourceAccount FromJson(const nlohmann::json& changeWriteResourceAccountJson);
private:
    std::string AuthenticationKey;
    std::string GuidCreationNum;
    std::string SequenceNumber;
};

class APTOS_API ChangeWriteResourceWriteTableItem : public Change {
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
class APTOS_API Transaction : public TransactionRequest {
public:
    explicit Transaction(const TransactionRequest& transactionRequest);
    Transaction();
    static Transaction FromJson(const nlohmann::json& j);
    nlohmann::json ToJson() const;

    std::string getType() const;
    void setType(const std::string &newType);

    bool getSuccess() const;
    void setSuccess(bool newSuccess);

    std::string getHash() const;

    std::string getVersion() const;
    void setVersion(const std::string &newVersion);

    void setHash(const std::string &newHash);

    std::string getStateChangeHash() const;
    void setStateChangeHash(const std::string &newStateChangeHash);

    std::string getEventRootHash() const;
    void setEventRootHash(const std::string &newEventRootHash);

    std::string getStateCheckpointHash() const;
    void setStateCheckpointHash(const std::string &newStateCheckpointHash);

    std::string getGasUsed() const;
    void setGasUsed(const std::string &newGasUsed);

    std::string getVmStatus() const;
    void setVmStatus(const std::string &newVmStatus);

    std::string getAccumulatorRootHash() const;
    void setAccumulatorRootHash(const std::string &newAccumulatorRootHash);

    std::string getTimestamp() const;
    void setTimestamp(const std::string &newTimestamp);

    std::vector<Change> getChanges() const;
    void setChanges(const std::vector<Change> &newChanges);

    std::vector<TransactionEvent> getEvents() const;
    void setEvents(const std::vector<TransactionEvent> &newEvents);

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
