#include "TableItemTokenMetadata.h"

namespace AptosRESTModel {

nlohmann::json Royalty::ToJson() const {
    nlohmann::json royaltyJson;
    royaltyJson["payee_address"] = PayeeAddress;
    royaltyJson["royalty_points_denominator"] = RoyaltyPointsDenominator;
    royaltyJson["royalty_points_numerator"] = RoyaltyPointsNumerator;
    return royaltyJson;
}

Royalty Royalty::FromJson(const nlohmann::json &royaltyJson) {
    Royalty royalty;
    royalty.PayeeAddress = royaltyJson["payee_address"];
    royalty.RoyaltyPointsDenominator = royaltyJson["royalty_points_denominator"];
    royalty.RoyaltyPointsNumerator = royaltyJson["royalty_points_numerator"];
    return royalty;
}

nlohmann::json MutabilityConfig::ToJson() const {
    nlohmann::json mutabilityConfigJson;
    mutabilityConfigJson["description"] = Description;
    mutabilityConfigJson["maximum"] = Maximum;
    mutabilityConfigJson["properties"] = Properties;
    mutabilityConfigJson["royalty"] = Royalty;
    mutabilityConfigJson["uri"] = Uri;
    return mutabilityConfigJson;
}

MutabilityConfig MutabilityConfig::FromJson(const nlohmann::json &mutabilityConfigJson) {
    MutabilityConfig mutabilityConfig;
    mutabilityConfig.Description = mutabilityConfigJson["description"];
    mutabilityConfig.Maximum = mutabilityConfigJson["maximum"];
    mutabilityConfig.Properties = mutabilityConfigJson["properties"];
    mutabilityConfig.Royalty = mutabilityConfigJson["royalty"];
    mutabilityConfig.Uri = mutabilityConfigJson["uri"];
    return mutabilityConfig;
}

nlohmann::json TableItemTokenMetadata::ToJson() const {
    nlohmann::json tokenMetadataJson;
    tokenMetadataJson["default_properties"] = TokenPropertiesProp.ToJson();
    tokenMetadataJson["description"] = Description;
    tokenMetadataJson["largest_property_version"] = LargestPropertyVersion;
    tokenMetadataJson["maximum"] = Maximum;
    tokenMetadataJson["mutability_config"] = MutabilityConfigProp.ToJson();
    tokenMetadataJson["name"] = Name;
    tokenMetadataJson["royalty"] = RoyaltyProp.ToJson();
    tokenMetadataJson["supply"] = Supply;
    tokenMetadataJson["uri"] = Uri;
    return tokenMetadataJson;
}

TableItemTokenMetadata TableItemTokenMetadata::FromJson(const nlohmann::json &tokenMetadataJson) {
    TableItemTokenMetadata tokenMetadata;
    tokenMetadata.TokenPropertiesProp = TokenProperties::FromJson(tokenMetadataJson["default_properties"]);
    tokenMetadata.Description = tokenMetadataJson["description"];
    tokenMetadata.LargestPropertyVersion = tokenMetadataJson["largest_property_version"];
    tokenMetadata.Maximum = tokenMetadataJson["maximum"];
    tokenMetadata.MutabilityConfigProp = MutabilityConfig::FromJson(tokenMetadataJson["mutability_config"]);
    tokenMetadata.Name = tokenMetadataJson["name"];
    tokenMetadata.RoyaltyProp = Royalty::FromJson(tokenMetadataJson["royalty"]);
    tokenMetadata.Supply = tokenMetadataJson["supply"];
    tokenMetadata.Uri = tokenMetadataJson["uri"];
    return tokenMetadata;
}

}
