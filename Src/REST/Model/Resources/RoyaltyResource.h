#ifndef ROYALTYRESOURCE_H
#define ROYALTYRESOURCE_H

#include <nlohmann/json.hpp>
#include "Base/ResourceDataBase.h"
#include "Base/IResourceBase.h"

namespace AptosRESTModel {

class APTOS_API RoyaltyResourceData : public ResourceDataBase {
public:
    nlohmann::json ToJson() const;
    static RoyaltyResourceData FromJson(const nlohmann::json& royaltyDataJson);
    std::string getDenominator() const;
    std::string getNumerator() const;
    std::string getPayeeAddress() const;

private:
    std::string Denominator;
    std::string Numerator;
    std::string PayeeAddress;
};

class APTOS_API RoyaltyResource : public IResourceBase {
public:
    nlohmann::json ToJson() const;
    static RoyaltyResource FromJson(const nlohmann::json& royaltyResourceJson);
    std::string getType() const override;
    void setType(const std::string &newType);

    RoyaltyResourceData getData() const;
    void setData(const RoyaltyResourceData &newData);

private:
    std::string Type;
    RoyaltyResourceData Data;
};
}
#endif // ROYALTYRESOURCE_H
