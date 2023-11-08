#ifndef VIEWREQUEST_H
#define VIEWREQUEST_H

#include <nlohmann/json.hpp>

namespace AptosRESTModel {
/// <summary>
/// Represents a view function request
/// </summary>
class ViewRequest {
public:
    nlohmann::json ToJson() const;
    static ViewRequest FromJson(const nlohmann::json& requestJson);
private:
    std::string Function;
    std::vector<std::string> TypeArguments;
    std::vector<std::string> Arguments;
};

}
#endif // VIEWREQUEST_H
