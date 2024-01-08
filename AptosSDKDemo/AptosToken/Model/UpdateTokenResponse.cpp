#include "UpdateTokenResponse.h"

namespace AptosSDKDemo
{
const std::string& UpdateTokenResponse::getHash() const
{
    return hash;
}

void UpdateTokenResponse::setHash(const std::string& newHash)
{
    hash = newHash;
}

const std::string& UpdateTokenResponse::getSender() const
{
    return sender;
}

void UpdateTokenResponse::setSender(const std::string& newSender)
{
    sender = newSender;
}

nlohmann::json UpdateTokenResponse::to_json() const
{
    return {
        {"hash", hash},
        {"sender", sender}
    };
}

UpdateTokenResponse UpdateTokenResponse::from_json(const nlohmann::json& j)
{
    UpdateTokenResponse response;
    response.setHash(j.value("hash", ""));
    response.setSender(j.value("sender", ""));
    return response;
}
}
