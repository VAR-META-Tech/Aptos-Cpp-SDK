#ifndef IRESOURCEBASE_H
#define IRESOURCEBASE_H

#include <nlohmann/json.hpp>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {

class APTOS_API IResourceBase {
public:
    explicit IResourceBase(const std::string& type);
    IResourceBase();
    virtual ~IResourceBase();
    nlohmann::json ToJson() const;
    static IResourceBase FromJson(const nlohmann::json& resourceJson);
    virtual std::string getType() const;
    void setType(const std::string &newType);

private:
    std::string Type;
};
}
#endif // IRESOURCEBASE_H
