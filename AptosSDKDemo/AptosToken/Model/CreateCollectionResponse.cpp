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


namespace AptosSDKDemo {
std::string CreateCollectionResponse::getHash() const
{
    return Hash;
}

void CreateCollectionResponse::setHash(const std::string &newHash)
{
    Hash = newHash;
}

std::string CreateCollectionResponse::getSender() const
{
    return Sender;
}

void CreateCollectionResponse::setSender(const std::string &newSender)
{
    Sender = newSender;
}

}
