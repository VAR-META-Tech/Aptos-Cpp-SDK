#ifndef TABLEITEMREQUEST_H
#define TABLEITEMREQUEST_H

#include <iostream>
#include <nlohmann/json.hpp>
namespace AptosRESTModel {
/// <summary>
/// Represents a table item request
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/TableItemRequest
/// </summary>
class TableItemRequest {
public:
    nlohmann::json ToJson() const;
    static TableItemRequest FromJson(const nlohmann::json& jsonData);
private:
    std::string KeyType;
    std::string ValueType;
    std::string Key;
};

}
#endif // TABLEITEMREQUEST_H
