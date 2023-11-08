#include "IResourceBase.h"

namespace AptosRESTModel {

IResourceBase::IResourceBase(const std::string &type) : Type(type) {}

IResourceBase::IResourceBase() : Type("") {}

nlohmann::json IResourceBase::ToJson() const {
    nlohmann::json resourceJson;
    resourceJson["type"] = Type;
    return resourceJson;
}

IResourceBase IResourceBase::FromJson(const nlohmann::json &resourceJson) {
    std::string type = resourceJson["type"];
    return IResourceBase(type);
}
}
