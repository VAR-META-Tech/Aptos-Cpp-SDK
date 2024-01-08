#include "RotateKeyBcsTransactionResponse.h"

namespace AptosSDKDemo
{
const std::string& RotateKeyBcsTransactionResponse::getHash() const
{
    return hash;
}

void RotateKeyBcsTransactionResponse::setHash(const std::string& newHash)
{
    hash = newHash;
}

const std::string& RotateKeyBcsTransactionResponse::getSender() const
{
    return sender;
}

void RotateKeyBcsTransactionResponse::setSender(const std::string& newSender)
{
    sender = newSender;
}

nlohmann::json RotateKeyBcsTransactionResponse::to_json() const
{
    return {
        {"hash", hash},
        {"sender", sender}
    };
}

RotateKeyBcsTransactionResponse RotateKeyBcsTransactionResponse::from_json(const nlohmann::json& j)
{
    RotateKeyBcsTransactionResponse response;
    response.setHash(j.value("hash", ""));
    response.setSender(j.value("sender", ""));
    return response;
}
}
