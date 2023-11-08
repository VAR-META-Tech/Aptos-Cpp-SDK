#ifndef TOKENDATAID_H
#define TOKENDATAID_H

#include <iostream>
#include <nlohmann/json.hpp>
namespace AptosRESTModel {

class TokenDataId {
public:
    nlohmann::json ToJson() const;
    static TokenDataId FromJson(const nlohmann::json& jsonData);
private:
    std::string collection;
    std::string creator;
    std::string name;
};

}
#endif // TOKENDATAID_H
