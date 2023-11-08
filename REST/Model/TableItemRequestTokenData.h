#ifndef TABLEITEMREQUESTTOKENDATA_H
#define TABLEITEMREQUESTTOKENDATA_H
#include <iostream>
#include <nlohmann/json.hpp>
#include "TokenDataId.h"
namespace AptosRESTModel {

/// <summary>
/// Represents a table item request
/// </summary>
class TableItemRequestTokenData {
public:
    nlohmann::json ToJson() const;
    static TableItemRequestTokenData FromJson(const nlohmann::json& jsonData);
private:
    std::string key_type;
    std::string value_type;
    TokenDataId key;
};

}
#endif // TABLEITEMREQUESTTOKENDATA_H
