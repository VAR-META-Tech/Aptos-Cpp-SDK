#include "CreateTokenResponse.h"


nlohmann::json AptosSDKDemo::CreateTokenResponse::to_json() const
{
    return {
        {"hash", Hash},
        {"sender", Sender}
    };
}

AptosSDKDemo::CreateTokenResponse AptosSDKDemo::CreateTokenResponse::from_json(const nlohmann::json &j)
{
    CreateTokenResponse response;
    response.Hash = j.value("hash", "");
    response.Sender = j.value("sender", "");
    return response;
}
