#ifndef TABLEITEMREQUESTNFT_H
#define TABLEITEMREQUESTNFT_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "TokenIdRequest.h"
namespace AptosRESTModel {
/// <summary>
/// Represents a table item request
/// </summary>
class TableItemRequestNFT {
public:
    nlohmann::json ToJson() const;
    static TableItemRequestNFT FromJson(const nlohmann::json& jsonData);
private:
    std::string KeyType;
    std::string ValueType;
    TokenIdRequest Key;
};

}
#endif // TABLEITEMREQUESTNFT_H
