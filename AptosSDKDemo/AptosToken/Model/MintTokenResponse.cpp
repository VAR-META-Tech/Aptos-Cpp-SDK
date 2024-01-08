#include "MintTokenResponse.h"


const std::string &AptosSDKDemo::MintTokenResponse::getHash() const
{
    return hash;
}

void AptosSDKDemo::MintTokenResponse::setHash(const std::string &newHash)
{
    hash = newHash;
}

const std::string &AptosSDKDemo::MintTokenResponse::getSender() const
{
    return sender;
}

void AptosSDKDemo::MintTokenResponse::setSender(const std::string &newSender)
{
    sender = newSender;
}

nlohmann::json AptosSDKDemo::MintTokenResponse::to_json() const
{
    return {
        {"hash", hash},
        {"sender", sender}
    };
}

AptosSDKDemo::MintTokenResponse AptosSDKDemo::MintTokenResponse::from_json(const nlohmann::json &j)
{
    MintTokenResponse response;
    response.setHash(j.value("hash", ""));
    response.setSender(j.value("sender", ""));
    return response;
}
