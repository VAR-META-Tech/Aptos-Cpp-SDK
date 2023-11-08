#include "ViewRequest.h"

namespace AptosRESTModel {

nlohmann::json ViewRequest::ToJson() const {
    nlohmann::json requestJson;
    requestJson["function"] = Function;
    requestJson["type_arguments"] = TypeArguments;
    if (!Arguments.empty()) {
        requestJson["arguments"] = Arguments;
    }
    return requestJson;
}

ViewRequest ViewRequest::FromJson(const nlohmann::json &requestJson) {
    ViewRequest request;
    request.Function = requestJson["function"];
    request.TypeArguments = requestJson["type_arguments"].get<std::vector<std::string>>();
    if (requestJson.find("arguments") != requestJson.end()) {
        request.Arguments = requestJson["arguments"].get<std::vector<std::string>>();
    }
    return request;
}
}
