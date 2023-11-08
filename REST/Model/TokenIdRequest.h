#ifndef TOKENIDREQUEST_H
#define TOKENIDREQUEST_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "TokenDataId.h"
namespace AptosRESTModel {

class TokenIdRequest {
public:
    nlohmann::json ToJson() const;
    static TokenIdRequest FromJson(const nlohmann::json& jsonData);
private:
    TokenDataId TokenDataIdProp;
    std::string PropertyVersion;
};

}
#endif // TOKENIDREQUEST_H
