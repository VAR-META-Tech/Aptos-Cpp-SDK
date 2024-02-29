#include "AccountResourceTokenStore.h"
namespace AptosRESTModel {

nlohmann::json AccountResourceTokenStore::ToJson() const {
    return nlohmann::json{
        {"type", Type},
        {"data", DataProp.ToJson()}
    };
}

AccountResourceTokenStore AccountResourceTokenStore::FromJson(const nlohmann::json &jsonData) {
    AccountResourceTokenStore resourceTokenStore;
    resourceTokenStore.Type = jsonData["type"];
    resourceTokenStore.DataProp = Data::FromJson(jsonData["data"]);

    return resourceTokenStore;
}

AccountResourceTokenStore::Data AccountResourceTokenStore::getDataProp() const
{
    return DataProp;
}

nlohmann::json AccountResourceTokenStore::Data::ToJson() const {
    return nlohmann::json{
        {"burn_events", BurnEvent.ToJson()},
        {"deposit_events", DepositEvents.ToJson()},
        {"direct_transfer", DirectTransfer},
        {"mutate_token_property_events", MutateTokenPropertyEventsProp.ToJson()},
        {"tokens", TokensProp.ToJson()}
    };
}

AccountResourceTokenStore::Data AccountResourceTokenStore::Data::FromJson(const nlohmann::json &jsonData) {
    Data data;
    data.BurnEvent = BurnEvents::FromJson(jsonData["burn_events"]);
    data.DepositEvents = DespositEvents::FromJson(jsonData["deposit_events"]);
    data.DirectTransfer = jsonData["direct_transfer"].dump();
    data.MutateTokenPropertyEventsProp = MutateTokenPropertyEvents::FromJson(jsonData["mutate_token_property_events"]);
    data.TokensProp = Tokens::FromJson(jsonData["tokens"]);
    return data;
}

AccountResourceTokenStore::Tokens AccountResourceTokenStore::Data::getTokensProp() const
{
    return TokensProp;
}

nlohmann::json AccountResourceTokenStore::BurnEvents::ToJson() const {
    return nlohmann::json{
        {"counter", Counter},
        {"guid", GuidProp.ToJson()}
    };
}

AccountResourceTokenStore::BurnEvents AccountResourceTokenStore::BurnEvents::FromJson(const nlohmann::json &jsonData) {
    BurnEvents events;
    events.Counter = jsonData["counter"];
    events.GuidProp = Guid::FromJson(jsonData["guid"]);
    return events;
}

nlohmann::json AccountResourceTokenStore::DespositEvents::ToJson() const {
    return nlohmann::json{
        {"counter", Counter},
        {"guid", GuidProp.ToJson()}
    };
}

AccountResourceTokenStore::DespositEvents AccountResourceTokenStore::DespositEvents::FromJson(const nlohmann::json &jsonData) {
    DespositEvents events;
    events.Counter = jsonData["counter"];
    events.GuidProp = Guid::FromJson(jsonData["guid"]);
    return events;
}

nlohmann::json AccountResourceTokenStore::MutateTokenPropertyEvents::ToJson() const {
    return nlohmann::json{
        {"counter", Counter},
        {"guid", GuidProp.ToJson()}
    };
}

AccountResourceTokenStore::MutateTokenPropertyEvents AccountResourceTokenStore::MutateTokenPropertyEvents::FromJson(const nlohmann::json &jsonData) {
    MutateTokenPropertyEvents events;
    events.Counter = jsonData["counter"];
    events.GuidProp = Guid::FromJson(jsonData["guid"]);
    return events;
}

nlohmann::json AccountResourceTokenStore::Guid::ToJson() const {
    return nlohmann::json{
        {"id", IdProp.ToJson()}
    };
}

AccountResourceTokenStore::Guid AccountResourceTokenStore::Guid::FromJson(const nlohmann::json &jsonData) {
    Guid guid;
    guid.IdProp = Id::FromJson(jsonData["id"]);
    return guid;
}

nlohmann::json AccountResourceTokenStore::Id::ToJson() const {
    return nlohmann::json{
        {"addr", Addr},
        {"creation_num", CreationNum}
    };
}

AccountResourceTokenStore::Id AccountResourceTokenStore::Id::FromJson(const nlohmann::json &jsonData) {
    Id id;
    id.Addr = jsonData["addr"];
    id.CreationNum = jsonData["creation_num"];
    return id;
}

nlohmann::json AccountResourceTokenStore::WithdrawEvents::ToJson() const {
    return nlohmann::json{
        {"counter", Counter},
        {"guid", GuidProp.ToJson()}
    };
}

AccountResourceTokenStore::WithdrawEvents AccountResourceTokenStore::WithdrawEvents::FromJson(const nlohmann::json &jsonData) {
    WithdrawEvents events;
    events.Counter = jsonData["counter"];
    events.GuidProp = Guid::FromJson(jsonData["guid"]);
    return events;
}

nlohmann::json AccountResourceTokenStore::Tokens::ToJson() const {
    return nlohmann::json{
        {"handle", Handle}
    };
}

AccountResourceTokenStore::Tokens AccountResourceTokenStore::Tokens::FromJson(const nlohmann::json &jsonData) {
    Tokens tokens;
    tokens.Handle = jsonData["handle"];
    return tokens;
}

std::string AccountResourceTokenStore::Tokens::getHandle() const
{
    return Handle;
}
}
