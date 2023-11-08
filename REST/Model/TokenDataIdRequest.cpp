#include "TokenDataIdRequest.h"

namespace AptosRESTModel {

nlohmann::json TokenDataIdRequest::ToJson() const {
    return nlohmann::json{
        {"creator", Creator},
        {"collection", Collection},
        {"name", Name}
    };
}

TokenDataIdRequest TokenDataIdRequest::FromJson(const nlohmann::json &jsonData) {
    TokenDataIdRequest tokenDataIdRequest;
    if (jsonData.find("creator") != jsonData.end()) {
        tokenDataIdRequest.Creator = jsonData["creator"].get<std::string>();
    }
    if (jsonData.find("collection") != jsonData.end()) {
        tokenDataIdRequest.Collection = jsonData["collection"].get<std::string>();
    }
    if (jsonData.find("name") != jsonData.end()) {
        tokenDataIdRequest.Name = jsonData["name"].get<std::string>();
    }
    return tokenDataIdRequest;
}

}
