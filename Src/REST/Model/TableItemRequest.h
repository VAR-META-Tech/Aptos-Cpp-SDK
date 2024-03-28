#ifndef TABLEITEMREQUEST_H
#define TABLEITEMREQUEST_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {
/// <summary>
/// Represents a table item request
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/TableItemRequest
/// </summary>
class APTOS_API TableItemRequest {
public:
    nlohmann::json ToJson() const;
    static TableItemRequest FromJson(const nlohmann::json& jsonData);
    std::string getKeyType() const;
    void setKeyType(const std::string &newKeyType);

    std::string getValueType() const;
    void setValueType(const std::string &newValueType);

    std::string getKey() const;
    void setKey(const std::string &newKey);

private:
    std::string KeyType;
    std::string ValueType;
    std::string Key;
};

}
#endif // TABLEITEMREQUEST_H
