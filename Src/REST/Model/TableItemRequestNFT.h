#ifndef TABLEITEMREQUESTNFT_H
#define TABLEITEMREQUESTNFT_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "TokenIdRequest.h"
namespace AptosRESTModel {
/// <summary>
/// Represents a table item request
/// </summary>
class APTOS_API TableItemRequestNFT {
public:
    nlohmann::json ToJson() const;
    static TableItemRequestNFT FromJson(const nlohmann::json& jsonData);
    std::string getKeyType() const;
    void setKeyType(const std::string &newKeyType);

    std::string getValueType() const;
    void setValueType(const std::string &newValueType);

    TokenIdRequest getKey() const;

    void setKey(const TokenIdRequest &newKey);

private:
    std::string KeyType;
    std::string ValueType;
    TokenIdRequest Key;
};

}
#endif // TABLEITEMREQUESTNFT_H
