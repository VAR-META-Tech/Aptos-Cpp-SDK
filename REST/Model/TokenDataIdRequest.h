#ifndef TOKENDATAIDREQUEST_H
#define TOKENDATAIDREQUEST_H

#include <iostream>
#include <nlohmann/json.hpp>
namespace AptosRESTModel {

class TokenDataIdRequest {
public:
    nlohmann::json ToJson() const;
    static TokenDataIdRequest FromJson(const nlohmann::json& jsonData);
private:
    std::string Creator;
    std::string Collection;
    std::string Name;
};

}
#endif // TOKENDATAIDREQUEST_H
