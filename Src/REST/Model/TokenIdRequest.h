#ifndef TOKENIDREQUEST_H
#define TOKENIDREQUEST_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "TokenDataId.h"
namespace AptosRESTModel {

class APTOS_API TokenIdRequest {
public:
    nlohmann::json ToJson() const;
    static TokenIdRequest FromJson(const nlohmann::json& jsonData);
    TokenDataId getTokenDataIdProp() const;

    void setTokenDataIdProp(const TokenDataId &newTokenDataIdProp);

    std::string getPropertyVersion() const;
    void setPropertyVersion(const std::string &newPropertyVersion);

private:
    TokenDataId TokenDataIdProp;
    std::string PropertyVersion;
};

}
#endif // TOKENIDREQUEST_H
