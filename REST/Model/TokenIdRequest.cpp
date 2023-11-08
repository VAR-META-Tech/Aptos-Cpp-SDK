#include "TokenIdRequest.h"

namespace AptosRESTModel {

nlohmann::json TokenIdRequest::ToJson() const {
    return nlohmann::json{
        {"token_data_id", TokenDataIdProp.ToJson()},
        {"property_version", PropertyVersion}
    };
}

TokenIdRequest TokenIdRequest::FromJson(const nlohmann::json &jsonData) {
    TokenIdRequest tokenIdRequest;
    if (jsonData.find("token_data_id") != jsonData.end()) {
        tokenIdRequest.TokenDataIdProp = TokenDataId::FromJson(jsonData["token_data_id"]);
    }
    if (jsonData.find("property_version") != jsonData.end()) {
        tokenIdRequest.PropertyVersion = jsonData["property_version"].get<std::string>();
    }
    return tokenIdRequest;
}

}
