#include "RoyaltyResource.h"

namespace AptosRESTModel {

nlohmann::json RoyaltyResource::ToJson() const {
    nlohmann::json royaltyResourceJson;
    royaltyResourceJson["type"] = Type;
    royaltyResourceJson["data"] = Data.ToJson();
    return royaltyResourceJson;
}

RoyaltyResource RoyaltyResource::FromJson(const nlohmann::json &royaltyResourceJson) {
    RoyaltyResource royaltyResource;
    royaltyResource.Type = royaltyResourceJson["type"];
    royaltyResource.Data = RoyaltyResourceData::FromJson(royaltyResourceJson["data"]);
    return royaltyResource;
}

std::string RoyaltyResource::getType() const
{
    return Type;
}

void RoyaltyResource::setType(const std::string &newType)
{
    Type = newType;
}

RoyaltyResourceData RoyaltyResource::getData() const
{
    return Data;
}

void RoyaltyResource::setData(const RoyaltyResourceData &newData)
{
    Data = newData;
}

nlohmann::json RoyaltyResourceData::ToJson() const {
    nlohmann::json royaltyDataJson;
    royaltyDataJson["denominator"] = Denominator;
    royaltyDataJson["numerator"] = Numerator;
    royaltyDataJson["payee_address"] = PayeeAddress;
    return royaltyDataJson;
}

RoyaltyResourceData RoyaltyResourceData::FromJson(const nlohmann::json &royaltyDataJson) {
    RoyaltyResourceData royaltyData;
    royaltyData.Denominator = royaltyDataJson["denominator"];
    royaltyData.Numerator = royaltyDataJson["numerator"];
    royaltyData.PayeeAddress = royaltyDataJson["payee_address"];
    return royaltyData;
}

std::string RoyaltyResourceData::getDenominator() const
{
    return Denominator;
}

std::string RoyaltyResourceData::getNumerator() const
{
    return Numerator;
}

std::string RoyaltyResourceData::getPayeeAddress() const
{
    return PayeeAddress;
}

}
