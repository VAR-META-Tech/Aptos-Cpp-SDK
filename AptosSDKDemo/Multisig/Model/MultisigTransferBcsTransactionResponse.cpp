#include "MultisigTransferBcsTransactionResponse.h"

namespace AptosSDKDemo
{
const std::string& MultisigTransferBcsTransactionResponse::getHash() const
{
    return hash;
}

void MultisigTransferBcsTransactionResponse::setHash(const std::string& newHash)
{
    hash = newHash;
}

const std::string& MultisigTransferBcsTransactionResponse::getSender() const
{
    return sender;
}

void MultisigTransferBcsTransactionResponse::setSender(const std::string& newSender)
{
    sender = newSender;
}

nlohmann::json MultisigTransferBcsTransactionResponse::to_json() const
{
    return {
        {"hash", hash},
        {"sender", sender}
    };
}

MultisigTransferBcsTransactionResponse MultisigTransferBcsTransactionResponse::from_json(const nlohmann::json& j)
{
    MultisigTransferBcsTransactionResponse response;
    response.setHash(j.value("hash", ""));
    response.setSender(j.value("sender", ""));
    return response;
}
}
