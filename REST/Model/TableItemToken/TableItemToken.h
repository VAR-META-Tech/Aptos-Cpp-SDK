#ifndef TABLEITEMTOKEN_H
#define TABLEITEMTOKEN_H
#include <nlohmann/json.hpp>
#include "../TokenDataId.h"

namespace AptosRESTModel {

class Map {
public:
    nlohmann::json ToJson() const;
    static Map FromJson(const nlohmann::json& mapJson);
private:
    std::vector<std::string> Data;
};

class TokenProperties {
public:
    nlohmann::json ToJson() const;
    static TokenProperties FromJson(const nlohmann::json& tokenPropertiesJson);
private:
    Map MapProp;
};

class Id {
public:
    nlohmann::json ToJson() const;
    static Id FromJson(const nlohmann::json& idJson);
private:
    TokenDataId TokenDataIdProp;
    std::string PropertyVersion;
};

class TableItemToken {
public:
    nlohmann::json ToJson() const;
    static TableItemToken FromJson(const nlohmann::json& tableItemTokenJson);
private:
    std::string Amount;
    Id IdProp;
    TokenProperties TokenPropertiesProp;
};
}
#endif // TABLEITEMTOKEN_H
