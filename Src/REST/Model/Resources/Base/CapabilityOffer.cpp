#include "CapabilityOffer.h"

namespace AptosRESTModel {
nlohmann::json CapabilityOffer::ToJson() const {
    nlohmann::json offerJson;
    offerJson["for"] = ForProp.ToJson();
    return offerJson;
}

CapabilityOffer CapabilityOffer::FromJson(const nlohmann::json &offerJson) {
    CapabilityOffer offer;
    offer.ForProp = For::FromJson(offerJson["for"]);
    return offer;
}

nlohmann::json For::ToJson() const {
    nlohmann::json forJson;
    forJson["vec"] = Vec;
    return forJson;
}

For For::FromJson(const nlohmann::json &forJson) {
    For forObject;
    forObject.Vec = forJson["vec"].get<std::vector<std::string>>();
    return forObject;
}
}
