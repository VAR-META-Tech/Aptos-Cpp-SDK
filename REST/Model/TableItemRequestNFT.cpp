#include "TableItemRequestNFT.h"

namespace AptosRESTModel {

nlohmann::json TableItemRequestNFT::ToJson() const {
    return nlohmann::json{
        {"key_type", KeyType},
        {"value_type", ValueType},
        {"key", Key.ToJson()}
    };
}

TableItemRequestNFT TableItemRequestNFT::FromJson(const nlohmann::json &jsonData) {
    TableItemRequestNFT request;
    if (jsonData.find("key_type") != jsonData.end()) {
        request.KeyType = jsonData["key_type"].get<std::string>();
    }
    if (jsonData.find("value_type") != jsonData.end()) {
        request.ValueType = jsonData["value_type"].get<std::string>();
    }
    if (jsonData.find("key") != jsonData.end()) {
        request.Key = TokenIdRequest::FromJson(jsonData["key"]);
    }
    return request;
}

std::string TableItemRequestNFT::getKeyType() const
{
    return KeyType;
}

void TableItemRequestNFT::setKeyType(const std::string &newKeyType)
{
    KeyType = newKeyType;
}

std::string TableItemRequestNFT::getValueType() const
{
    return ValueType;
}

void TableItemRequestNFT::setValueType(const std::string &newValueType)
{
    ValueType = newValueType;
}

TokenIdRequest TableItemRequestNFT::getKey() const
{
    return Key;
}

void TableItemRequestNFT::setKey(const TokenIdRequest &newKey)
{
    Key = newKey;
}
}
