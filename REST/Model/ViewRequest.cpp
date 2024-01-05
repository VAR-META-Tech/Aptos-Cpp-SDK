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

std::string ViewRequest::getFunction() const
{
    return Function;
}

void ViewRequest::setFunction(const std::string &newFunction)
{
    Function = newFunction;
}

std::vector<std::string> ViewRequest::getTypeArguments() const
{
    return TypeArguments;
}

void ViewRequest::setTypeArguments(const std::vector<std::string> &newTypeArguments)
{
    TypeArguments = newTypeArguments;
}

std::vector<std::string> ViewRequest::getArguments() const
{
    return Arguments;
}

void ViewRequest::setArguments(const std::vector<std::string> &newArguments)
{
    Arguments = newArguments;
}
}
