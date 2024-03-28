#ifndef TOKENDATAID_H
#define TOKENDATAID_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {

class APTOS_API TokenDataId {
public:
    nlohmann::json ToJson() const;
    static TokenDataId FromJson(const nlohmann::json& jsonData);
    std::string getCollection() const;
    void setCollection(const std::string &newCollection);

    std::string getCreator() const;
    void setCreator(const std::string &newCreator);

    std::string getName() const;
    void setName(const std::string &newName);

private:
    std::string collection;
    std::string creator;
    std::string name;
};

}
#endif // TOKENDATAID_H
