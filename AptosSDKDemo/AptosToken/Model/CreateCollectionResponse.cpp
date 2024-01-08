#include "CreateCollectionResponse.h"


nlohmann::json AptosSDKDemo::CreateCollectionResponse::to_json() const
{
    return {
        {"hash", Hash},
        {"sender", Sender}
    };
}

AptosSDKDemo::CreateCollectionResponse AptosSDKDemo::CreateCollectionResponse::from_json(const nlohmann::json &j)
{
    CreateCollectionResponse response;
    response.Hash = j.value("hash", "");
    response.Sender = j.value("sender", "");
    return response;
}
