#include "AccountResourceCoin.h"
#include "iostream"

namespace AptosRESTModel {

AccountResourceCoin::Id::Id() {}

AccountResourceCoin::Id AccountResourceCoin::Id::FromJson(const nlohmann::json &jsonData) {
    Id id;
    id.m_addr = jsonData["addr"];
    id.m_creationNum = jsonData["creation_num"];
    return id;
}

nlohmann::json AccountResourceCoin::Id::ToJson() const {
    return nlohmann::json{
        {"addr", m_addr},
        {"creation_num", m_creationNum}
    };
}

AccountResourceCoin::AccountResourceCoin() {}

AccountResourceCoin AccountResourceCoin::FromJson(const nlohmann::json &jsonData) {
    AccountResourceCoin accountResource;
    accountResource.m_type = jsonData["type"];
    accountResource.m_dataProp = Data::FromJson(jsonData["data"]);

    return accountResource;
}

nlohmann::json AccountResourceCoin::ToJson() const {
    return nlohmann::json{
        {"type", m_type},
        {"data", m_dataProp.ToJson()}
    };
}

AccountResourceCoin::Data AccountResourceCoin::dataProp() const
{
    return m_dataProp;
}

AccountResourceCoin::Guid::Guid() {}

AccountResourceCoin::Guid AccountResourceCoin::Guid::FromJson(const nlohmann::json &jsonData) {
    Guid guid;
    guid.m_idProp = Id::FromJson(jsonData["id"]);
    return guid;
}

nlohmann::json AccountResourceCoin::Guid::ToJson() const {
    return nlohmann::json{
        {"id", m_idProp.ToJson()}
    };
}

AccountResourceCoin::WithdrawEvents::WithdrawEvents() {}

AccountResourceCoin::WithdrawEvents AccountResourceCoin::WithdrawEvents::FromJson(const nlohmann::json &jsonData) {
    WithdrawEvents events;
    events.m_counter = jsonData["counter"];
    events.m_guidProp = Guid::FromJson(jsonData["guid"]);
    return events;
}

nlohmann::json AccountResourceCoin::WithdrawEvents::ToJson() const {
    return nlohmann::json{
        {"counter", m_counter},
        {"guid", m_guidProp.ToJson()}
    };
}

AccountResourceCoin::DespositEvents::DespositEvents() {}

AccountResourceCoin::DespositEvents AccountResourceCoin::DespositEvents::FromJson(const nlohmann::json &jsonData) {
    DespositEvents events;
    events.m_counter = jsonData["counter"];
    events.m_guidProp = Guid::FromJson(jsonData["guid"]);
    return events;
}

nlohmann::json AccountResourceCoin::DespositEvents::ToJson() const {
    return nlohmann::json{
        {"counter", m_counter},
        {"guid", m_guidProp.ToJson()}
    };
}

AccountResourceCoin::Data AccountResourceCoin::Data::FromJson(const nlohmann::json &jsonData) {
    Data data;
    data.m_coinProp = Coin::FromJson(jsonData["coin"]);
    data.m_depositEvents = DespositEvents::FromJson(jsonData["deposit_events"]);
    data.m_frozen = jsonData["frozen"];
    data.m_withdrawEvents = WithdrawEvents::FromJson(jsonData["withdraw_events"]);
    return data;
}

nlohmann::json AccountResourceCoin::Data::ToJson() const {
    return nlohmann::json{
        {"coin", m_coinProp.ToJson()},
        {"deposit_events", m_depositEvents.ToJson()},
        {"frozen", m_frozen},
        {"withdraw_events", m_withdrawEvents.ToJson()}
    };
}

AccountResourceCoin::Coin AccountResourceCoin::Data::coinProp() const
{
    return m_coinProp;
}

AccountResourceCoin::Coin::Coin() {}

AccountResourceCoin::Coin AccountResourceCoin::Coin::FromJson(const nlohmann::json &jsonData) {
    Coin coin;
    coin.m_value = jsonData["value"];
    return coin;
}

nlohmann::json AccountResourceCoin::Coin::ToJson() const {
    return nlohmann::json{
        {"value", m_value}
    };
}

std::string AccountResourceCoin::Coin::value() const
{
    return m_value;
}

}
