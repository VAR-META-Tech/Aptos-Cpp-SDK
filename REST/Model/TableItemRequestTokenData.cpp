#include "TableItemRequestTokenData.h"

namespace AptosRESTModel {

nlohmann::json TableItemRequestTokenData::ToJson() const {
    return nlohmann::json{
        {"key_type", key_type},
        {"value_type", value_type},
        {"key", key.ToJson()}
    };
}

TableItemRequestTokenData TableItemRequestTokenData::FromJson(const nlohmann::json &jsonData) {
    TableItemRequestTokenData request;
    request.key_type = jsonData["key_type"].get<std::string>();
    request.value_type = jsonData["value_type"].get<std::string>();
    request.key = TokenDataId::FromJson(jsonData["key"]);
    return request;
}

std::string TableItemRequestTokenData::getKey_type() const
{
    return key_type;
}

void TableItemRequestTokenData::setKey_type(const std::string &newKey_type)
{
    key_type = newKey_type;
}

std::string TableItemRequestTokenData::getValue_type() const
{
    return value_type;
}

void TableItemRequestTokenData::setValue_type(const std::string &newValue_type)
{
    value_type = newValue_type;
}

TokenDataId TableItemRequestTokenData::getKey() const
{
    return key;
}

void TableItemRequestTokenData::setKey(const TokenDataId &newKey)
{
    key = newKey;
}

}
