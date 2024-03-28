#ifndef TABLEITEMTOKEN_H
#define TABLEITEMTOKEN_H
#include <nlohmann/json.hpp>
#include "../TokenDataId.h"

namespace AptosRESTModel {

class APTOS_API Map {
public:
    nlohmann::json ToJson() const;
    static Map FromJson(const nlohmann::json& mapJson);
private:
    std::vector<std::string> Data;
};

class APTOS_API TokenProperties {
public:
    nlohmann::json ToJson() const;
    static TokenProperties FromJson(const nlohmann::json& tokenPropertiesJson);
private:
    Map MapProp;
};

class APTOS_API Id {
public:
    nlohmann::json ToJson() const;
    static Id FromJson(const nlohmann::json& idJson);
    TokenDataId& getTokenDataIdProp();

private:
    TokenDataId TokenDataIdProp;
    std::string PropertyVersion;
};

/// <summary>
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/operations/get_table_item
/// </summary>
class APTOS_API TableItemToken {
public:
    nlohmann::json ToJson() const;
    static TableItemToken FromJson(const nlohmann::json& tableItemTokenJson);
    Id& getIdProp();

    std::string getAmount() const;
    void setAmount(const std::string &newAmount);

private:
    std::string Amount;
    Id IdProp;
    TokenProperties TokenPropertiesProp;
};
}
#endif // TABLEITEMTOKEN_H
