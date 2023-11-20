#include "TableItemRequest.h"

namespace AptosRESTModel {

nlohmann::json TableItemRequest::ToJson() const {
    return nlohmann::json{
        {"key_type", KeyType},
        {"value_type", ValueType},
        {"key", Key}
    };
}

TableItemRequest TableItemRequest::FromJson(const nlohmann::json &jsonData) {
    TableItemRequest request;
    if (jsonData.find("key_type") != jsonData.end()) {
        request.KeyType = jsonData["key_type"].get<std::string>();
    }
    if (jsonData.find("value_type") != jsonData.end()) {
        request.ValueType = jsonData["value_type"].get<std::string>();
    }
    if (jsonData.find("key") != jsonData.end()) {
        request.Key = jsonData["key"].get<std::string>();
    }
    return request;
}

std::string TableItemRequest::getKeyType() const
{
    return KeyType;
}

void TableItemRequest::setKeyType(const std::string &newKeyType)
{
    KeyType = newKeyType;
}

std::string TableItemRequest::getValueType() const
{
    return ValueType;
}

void TableItemRequest::setValueType(const std::string &newValueType)
{
    ValueType = newValueType;
}

std::string TableItemRequest::getKey() const
{
    return Key;
}

void TableItemRequest::setKey(const std::string &newKey)
{
    Key = newKey;
}

}
