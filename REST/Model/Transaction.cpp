#include "Transaction.h"

namespace AptosRESTModel {

TransactionEvent::DataObject TransactionEvent::DataObject::FromJson(const nlohmann::json &j) {
    DataObject dataObject;
    dataObject.Index = j.value("index", "");
    dataObject.Token = j.value("token", "");
    dataObject.From = j.value("from", "");
    dataObject.To = j.value("to", "");
    dataObject.Object = j.value("object", "");
    return dataObject;
}

nlohmann::json TransactionEvent::DataObject::ToJson() const {
    nlohmann::json j;
    j["index"] = Index;
    j["token"] = Token;
    j["from"] = From;
    j["to"] = To;
    j["object"] = Object;
    return j;
}

TransactionEvent::GUIDAddress TransactionEvent::GUIDAddress::FromJson(const nlohmann::json &j) {
    GUIDAddress guidAddress;
    guidAddress.CreationNumber = j.value("creation_number", "");
    guidAddress.AccountAddress = j.value("account_address", "");
    return guidAddress;
}

nlohmann::json TransactionEvent::GUIDAddress::ToJson() const {
    nlohmann::json j;
    j["creation_number"] = CreationNumber;
    j["account_address"] = AccountAddress;
    return j;
}

TransactionEvent TransactionEvent::FromJson(const nlohmann::json &j) {
    TransactionEvent transactionEvent;
    transactionEvent.GUID = GUIDAddress::FromJson(j["guid"]);
    transactionEvent.SequenceNumber = j.value("sequence_number", "");
    transactionEvent.Type = j.value("type", "");
    transactionEvent.Data = DataObject::FromJson(j["data"]);
    return transactionEvent;
}

nlohmann::json TransactionEvent::ToJson() const {
    nlohmann::json j;
    j["guid"] = GUID.ToJson();
    j["sequence_number"] = SequenceNumber;
    j["type"] = Type;
    j["data"] = Data.ToJson();
    return j;
}

Transaction::Transaction(const TransactionRequest &transactionRequest) : TransactionRequest(transactionRequest) {}

Transaction::Transaction() : TransactionRequest() {}

Transaction Transaction::FromJson(const nlohmann::json &j) {
    Transaction transaction;

    // Populate base class fields (assuming TransactionRequest is a base class)
    transaction.TransactionRequest::FromJson(j);

    transaction.Type = j.value("type", "");
    transaction.Version = j.value("version", "");
    transaction.Hash = j.value("hash", "");
    transaction.StateChangeHash = j.value("state_change_hash", "");
    transaction.EventRootHash = j.value("event_root_hash", "");
    transaction.StateCheckpointHash = j.value("state_checkpoint_hash", "");
    transaction.GasUsed = j.value("gas_used", "");
    transaction.Success = j.value("success", false);
    transaction.VmStatus = j.value("vm_status", "");
    transaction.AccumulatorRootHash = j.value("accumulator_root_hash", "");
    transaction.Timestamp = j.value("timestamp", "");

    for (const auto& change : j["changes"]) {
        transaction.Changes.push_back(Change::FromJson(change));
    }

    for (const auto& event : j["events"]) {
        transaction.Events.push_back(TransactionEvent::FromJson(event));
    }

    return transaction;
}

nlohmann::json Transaction::ToJson() const {
    nlohmann::json j = TransactionRequest::ToJson();

    j["type"] = Type;
    j["version"] = Version;
    j["hash"] = Hash;
    j["state_change_hash"] = StateChangeHash;
    j["event_root_hash"] = EventRootHash;
    j["state_checkpoint_hash"] = StateCheckpointHash;
    j["gas_used"] = GasUsed;
    j["success"] = Success;
    j["vm_status"] = VmStatus;
    j["accumulator_root_hash"] = AccumulatorRootHash;
    j["timestamp"] = Timestamp;

    nlohmann::json changesArray;
    for (const auto& change : Changes) {
        changesArray.push_back(change.ToJson());
    }
    j["changes"] = changesArray;

    nlohmann::json eventsArray;
    for (const auto& event : Events) {
        eventsArray.push_back(event.ToJson());
    }
    j["events"] = eventsArray;

    return j;
}

nlohmann::json ChangeWriteResourceWriteTableItem::ToJson() const {
    nlohmann::json changeWriteResourceWriteTableItemJson = Change::ToJson();
    changeWriteResourceWriteTableItemJson["state_key_hash"] = StateKeyHash;
    changeWriteResourceWriteTableItemJson["handle"] = Handle;
    changeWriteResourceWriteTableItemJson["key"] = Key;
    changeWriteResourceWriteTableItemJson["value"] = Value;
    changeWriteResourceWriteTableItemJson["data"] = Data;
    return changeWriteResourceWriteTableItemJson;
}

ChangeWriteResourceWriteTableItem ChangeWriteResourceWriteTableItem::FromJson(const nlohmann::json &changeWriteResourceWriteTableItemJson) {
    ChangeWriteResourceWriteTableItem changeWriteResourceWriteTableItem;
    changeWriteResourceWriteTableItem.Type = changeWriteResourceWriteTableItemJson["type"];
    changeWriteResourceWriteTableItem.StateKeyHash = changeWriteResourceWriteTableItemJson["state_key_hash"];
    changeWriteResourceWriteTableItem.Handle = changeWriteResourceWriteTableItemJson["handle"];
    changeWriteResourceWriteTableItem.Key = changeWriteResourceWriteTableItemJson["key"];
    changeWriteResourceWriteTableItem.Value = changeWriteResourceWriteTableItemJson["value"];
    changeWriteResourceWriteTableItem.Data = changeWriteResourceWriteTableItemJson["data"];
    return changeWriteResourceWriteTableItem;
}

nlohmann::json ChangeWriteResourceAccount::ToJson() const {
    nlohmann::json changeWriteResourceAccountJson = ChangeWriteResource::ToJson();
    changeWriteResourceAccountJson["authentication_key"] = AuthenticationKey;
    changeWriteResourceAccountJson["guid_creation_num"] = GuidCreationNum;
    changeWriteResourceAccountJson["sequence_number"] = SequenceNumber;
    return changeWriteResourceAccountJson;
}

ChangeWriteResourceAccount ChangeWriteResourceAccount::FromJson(const nlohmann::json &changeWriteResourceAccountJson) {
    ChangeWriteResourceAccount changeWriteResourceAccount;
    changeWriteResourceAccount.Type = changeWriteResourceAccountJson["type"];
    changeWriteResourceAccount.Address = changeWriteResourceAccountJson["Address"];
    changeWriteResourceAccount.StateKeyHash = changeWriteResourceAccountJson["state_key_hash"];
    changeWriteResourceAccount.AuthenticationKey = changeWriteResourceAccountJson["authentication_key"];
    changeWriteResourceAccount.GuidCreationNum = changeWriteResourceAccountJson["guid_creation_num"];
    changeWriteResourceAccount.SequenceNumber = changeWriteResourceAccountJson["sequence_number"];
    return changeWriteResourceAccount;
}

nlohmann::json ChangeWriteResource::ToJson() const {
    nlohmann::json changeWriteResourceJson = Change::ToJson();
    changeWriteResourceJson["Address"] = Address;
    changeWriteResourceJson["state_key_hash"] = StateKeyHash;
    return changeWriteResourceJson;
}

ChangeWriteResource ChangeWriteResource::FromJson(const nlohmann::json &changeWriteResourceJson) {
    ChangeWriteResource changeWriteResource;
    changeWriteResource.Type = changeWriteResourceJson["type"];
    changeWriteResource.Address = changeWriteResourceJson["Address"];
    changeWriteResource.StateKeyHash = changeWriteResourceJson["state_key_hash"];
    return changeWriteResource;
}

nlohmann::json Change::ToJson() const {
    nlohmann::json changeJson;
    changeJson["type"] = Type;
    return changeJson;
}

Change Change::FromJson(const nlohmann::json &changeJson) {
    Change change;
    change.Type = changeJson["type"];
    return change;
}

}
