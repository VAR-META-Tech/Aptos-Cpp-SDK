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

}
