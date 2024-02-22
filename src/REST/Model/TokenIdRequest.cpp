#include "TokenIdRequest.h"

namespace AptosRESTModel {

nlohmann::json TokenIdRequest::ToJson() const {
    return nlohmann::json{
        {"token_data_id", TokenDataIdProp.ToJson()},
        {"property_version", PropertyVersion}
    };
}

/// <summary>
/// A representation a token_id object
/// {
///     "token_data_id":{
///         "creator":"0xcd7820caacab04fbf1d7e563f4d329f06d2ce3140d654729d99258b5b68a27bf",
///         "collection":"Alice's",
///         "name":"Alice's first token"
///     },
///     "property_version":"0"
/// }
/// </summary>
TokenIdRequest TokenIdRequest::FromJson(const nlohmann::json &jsonData) {
    TokenIdRequest tokenIdRequest;
    if (jsonData.find("token_data_id") != jsonData.end()) {
        tokenIdRequest.TokenDataIdProp = TokenDataId::FromJson(jsonData["token_data_id"]);
    }
    if (jsonData.find("property_version") != jsonData.end()) {
        tokenIdRequest.PropertyVersion = jsonData["property_version"].get<std::string>();
    }
    return tokenIdRequest;
}

TokenDataId TokenIdRequest::getTokenDataIdProp() const
{
    return TokenDataIdProp;
}

void TokenIdRequest::setTokenDataIdProp(const TokenDataId &newTokenDataIdProp)
{
    TokenDataIdProp = newTokenDataIdProp;
}

std::string TokenIdRequest::getPropertyVersion() const
{
    return PropertyVersion;
}

void TokenIdRequest::setPropertyVersion(const std::string &newPropertyVersion)
{
    PropertyVersion = newPropertyVersion;
}

}
