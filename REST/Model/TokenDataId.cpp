#include "TokenDataId.h"

namespace AptosRESTModel {

nlohmann::json TokenDataId::ToJson() const {
    return nlohmann::json{
        {"collection", collection},
        {"creator", creator},
        {"name", name}
    };
}

TokenDataId TokenDataId::FromJson(const nlohmann::json &jsonData) {
    TokenDataId tokenDataId;
    if (jsonData.find("collection") != jsonData.end()) {
        tokenDataId.collection = jsonData["collection"].get<std::string>();
    }
    if (jsonData.find("creator") != jsonData.end()) {
        tokenDataId.creator = jsonData["creator"].get<std::string>();
    }
    if (jsonData.find("name") != jsonData.end()) {
        tokenDataId.name = jsonData["name"].get<std::string>();
    }
    return tokenDataId;
}

}
