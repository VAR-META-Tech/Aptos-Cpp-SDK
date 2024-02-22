#include "AccountResource.h"

namespace AptosRESTModel {

nlohmann::json AccountResourceData::ToJson() const {
    nlohmann::json dataJson;
    dataJson["authentication_key"] = AuthenticationKey;
    dataJson["coin_register_events"] = CoinRegisterEvents.ToJson();
    dataJson["guid_creation_num"] = GuidCreationNum;
    dataJson["key_rotation_events"] = KeyRotationEvents.ToJson();
    dataJson["rotation_capability_offer"] = RotationCapabilityOffer.ToJson();
    dataJson["sequence_number"] = SequenceNumber;
    dataJson["signer_capability_offer"] = SignerCapabilityOffer.ToJson();
    return dataJson;
}

AccountResourceData AccountResourceData::FromJson(const nlohmann::json &dataJson) {
    AccountResourceData data;
    data.AuthenticationKey = dataJson["authentication_key"];
    data.CoinRegisterEvents = ResourceEvent::FromJson(dataJson["coin_register_events"]);
    data.GuidCreationNum = dataJson["guid_creation_num"];
    data.KeyRotationEvents = ResourceEvent::FromJson(dataJson["key_rotation_events"]);
    data.RotationCapabilityOffer = CapabilityOffer::FromJson(dataJson["rotation_capability_offer"]);
    data.SequenceNumber = dataJson["sequence_number"];
    data.SignerCapabilityOffer = CapabilityOffer::FromJson(dataJson["signer_capability_offer"]);
    return data;
}

std::string AccountResourceData::getGuidCreationNum() const
{
    return GuidCreationNum;
}

void AccountResourceData::setGuidCreationNum(const std::string &newGuidCreationNum)
{
    GuidCreationNum = newGuidCreationNum;
}

nlohmann::json AccountResource::ToJson() const {
    nlohmann::json resourceJson;
    resourceJson["data"] = Data.ToJson();
    return resourceJson;
}

AccountResource AccountResource::FromJson(const nlohmann::json &resourceJson) {
    AccountResource resource;
    resource.Data = AccountResourceData::FromJson(resourceJson["data"]);
    return resource;
}

AccountResourceData AccountResource::getData() const
{
    return Data;
}

void AccountResource::setData(const AccountResourceData &newData)
{
    Data = newData;
}
}
