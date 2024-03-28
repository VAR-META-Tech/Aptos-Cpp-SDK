#ifndef TABLEITEMREQUESTTOKENDATA_H
#define TABLEITEMREQUESTTOKENDATA_H
#include <iostream>
#include <nlohmann/json.hpp>
#include "TokenDataId.h"
namespace AptosRESTModel {

/// <summary>
/// Represents a table item request
/// </summary>
class APTOS_API TableItemRequestTokenData {
public:
    nlohmann::json ToJson() const;
    static TableItemRequestTokenData FromJson(const nlohmann::json& jsonData);
    std::string getKey_type() const;
    void setKey_type(const std::string &newKey_type);

    std::string getValue_type() const;
    void setValue_type(const std::string &newValue_type);

    TokenDataId getKey() const;
    void setKey(const TokenDataId &newKey);

private:
    std::string key_type;
    std::string value_type;
    TokenDataId key;
};

}
#endif // TABLEITEMREQUESTTOKENDATA_H
