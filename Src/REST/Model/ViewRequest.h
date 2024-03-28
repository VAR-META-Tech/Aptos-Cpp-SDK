#ifndef VIEWREQUEST_H
#define VIEWREQUEST_H

#include <nlohmann/json.hpp>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {
/// <summary>
/// Represents a view function request
/// </summary>
class APTOS_API ViewRequest {
public:
    nlohmann::json ToJson() const;
    static ViewRequest FromJson(const nlohmann::json& requestJson);
    std::string getFunction() const;
    void setFunction(const std::string &newFunction);

    std::vector<std::string> getTypeArguments() const;
    void setTypeArguments(const std::vector<std::string> &newTypeArguments);

    std::vector<std::string> getArguments() const;
    void setArguments(const std::vector<std::string> &newArguments);

private:
    std::string Function;
    std::vector<std::string> TypeArguments;
    std::vector<std::string> Arguments;
};

}
#endif // VIEWREQUEST_H
