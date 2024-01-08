#include "TransferObjectResponse.h"

namespace AptosSDKDemo
{
const std::string& TransferObjectResponse::getHash() const
{
    return hash;
}

void TransferObjectResponse::setHash(const std::string& newHash)
{
    hash = newHash;
}

const std::string& TransferObjectResponse::getSender() const
{
    return sender;
}

void TransferObjectResponse::setSender(const std::string& newSender)
{
    sender = newSender;
}

nlohmann::json TransferObjectResponse::to_json() const
{
    return {
        {"hash", hash},
        {"sender", sender}
    };
}

TransferObjectResponse TransferObjectResponse::from_json(const nlohmann::json& j)
{
    TransferObjectResponse response;
    response.setHash(j.value("hash", ""));
    response.setSender(j.value("sender", ""));
    return response;
}
}
