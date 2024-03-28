#ifndef CAPABILITYOFFER_H
#define CAPABILITYOFFER_H

#include <nlohmann/json.hpp>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {
class APTOS_API For {
public:
    nlohmann::json ToJson() const;
    static For FromJson(const nlohmann::json& forJson);
private:
    std::vector<std::string> Vec;
};

class APTOS_API CapabilityOffer {
public:
    nlohmann::json ToJson() const;
    static CapabilityOffer FromJson(const nlohmann::json& offerJson);
private:
    For ForProp;
};
}

#endif // CAPABILITYOFFER_H
