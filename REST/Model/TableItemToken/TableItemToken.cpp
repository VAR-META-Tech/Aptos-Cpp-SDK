#include "TableItemToken.h"

namespace AptosRESTModel {

nlohmann::json Map::ToJson() const {
    nlohmann::json mapJson;
    mapJson["data"] = Data;
    return mapJson;
}

Map Map::FromJson(const nlohmann::json &mapJson) {
    Map map;
    map.Data = mapJson["data"].get<std::vector<std::string>>();
    return map;
}

nlohmann::json TokenProperties::ToJson() const {
    nlohmann::json tokenPropertiesJson;
    tokenPropertiesJson["map"] = MapProp.ToJson();
    return tokenPropertiesJson;
}

TokenProperties TokenProperties::FromJson(const nlohmann::json &tokenPropertiesJson) {
    TokenProperties tokenProperties;
    tokenProperties.MapProp = Map::FromJson(tokenPropertiesJson["map"]);
    return tokenProperties;
}

nlohmann::json Id::ToJson() const {
    nlohmann::json idJson;
    idJson["token_data_id"] = TokenDataIdProp.ToJson();
    idJson["property_version"] = PropertyVersion;
    return idJson;
}

Id Id::FromJson(const nlohmann::json &idJson) {
    Id id;
    id.TokenDataIdProp = TokenDataId::FromJson(idJson["token_data_id"]);
    id.PropertyVersion = idJson["property_version"];
    return id;
}

TokenDataId &Id::getTokenDataIdProp()
{
    return TokenDataIdProp;
}

nlohmann::json TableItemToken::ToJson() const {
    nlohmann::json tableItemTokenJson;
    tableItemTokenJson["amount"] = Amount;
    tableItemTokenJson["id"] = IdProp.ToJson();
    tableItemTokenJson["token_properties"] = TokenPropertiesProp.ToJson();
    return tableItemTokenJson;
}

TableItemToken TableItemToken::FromJson(const nlohmann::json &tableItemTokenJson) {
    TableItemToken tableItemToken;
    tableItemToken.Amount = tableItemTokenJson["amount"];
    tableItemToken.IdProp = Id::FromJson(tableItemTokenJson["id"]);
    tableItemToken.TokenPropertiesProp = TokenProperties::FromJson(tableItemTokenJson["token_properties"]);
    return tableItemToken;
}

Id &TableItemToken::getIdProp()
{
    return IdProp;
}

std::string TableItemToken::getAmount() const
{
    return Amount;
}

void TableItemToken::setAmount(const std::string &newAmount)
{
    Amount = newAmount;
}

}
