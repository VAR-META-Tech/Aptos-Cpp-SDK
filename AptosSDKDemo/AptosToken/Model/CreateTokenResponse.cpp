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


namespace AptosSDKDemo {
std::string CreateTokenResponse::getHash() const
{
    return Hash;
}

void CreateTokenResponse::setHash(const std::string &newHash)
{
    Hash = newHash;
}

std::string CreateTokenResponse::getSender() const
{
    return Sender;
}

void CreateTokenResponse::setSender(const std::string &newSender)
{
    Sender = newSender;
}

}
