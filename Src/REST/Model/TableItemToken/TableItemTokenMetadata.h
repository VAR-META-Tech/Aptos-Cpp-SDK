#ifndef TABLEITEMTOKENMETADATA_H
#define TABLEITEMTOKENMETADATA_H

#include <nlohmann/json.hpp>
#include "TableItemToken.h"

namespace AptosRESTModel {

class APTOS_API MutabilityConfig {
public:
    nlohmann::json ToJson() const;
    static MutabilityConfig FromJson(const nlohmann::json& mutabilityConfigJson);
private:
    bool Description;
    bool Maximum;
    bool Properties;
    bool Royalty;
    bool Uri;
};

class APTOS_API Royalty {
public:
    nlohmann::json ToJson() const;
    static Royalty FromJson(const nlohmann::json& royaltyJson);
private:
    std::string PayeeAddress;
    std::string RoyaltyPointsDenominator;
    std::string RoyaltyPointsNumerator;
};

class APTOS_API TableItemTokenMetadata {
public:
    nlohmann::json ToJson() const;
    static TableItemTokenMetadata FromJson(const nlohmann::json& tokenMetadataJson);
private:
    TokenProperties TokenPropertiesProp;
    std::string Description;
    std::string LargestPropertyVersion;
    std::string Maximum;
    MutabilityConfig MutabilityConfigProp;
    std::string Name;
    Royalty RoyaltyProp;
    std::string Supply;
    std::string Uri;
};

}
#endif // TABLEITEMTOKENMETADATA_H
