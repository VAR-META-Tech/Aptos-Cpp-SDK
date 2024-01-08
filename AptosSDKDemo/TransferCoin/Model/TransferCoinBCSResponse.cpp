#include "TransferCoinBCSResponse.h"

namespace AptosSDKDemo
{
const std::string& TransferCoinBCSResponse::getHash() const
{
    return hash;
}

void TransferCoinBCSResponse::setHash(const std::string& newHash)
{
    hash = newHash;
}

const std::string& TransferCoinBCSResponse::getSender() const
{
    return sender;
}

void TransferCoinBCSResponse::setSender(const std::string& newSender)
{
    sender = newSender;
}

const std::string& TransferCoinBCSResponse::getSequenceNumber() const
{
    return sequenceNumber;
}

void TransferCoinBCSResponse::setSequenceNumber(const std::string& newSequenceNumber)
{
    sequenceNumber = newSequenceNumber;
}

const std::string& TransferCoinBCSResponse::getMaxGasAmount() const
{
    return maxGasAmount;
}

void TransferCoinBCSResponse::setMaxGasAmount(const std::string& newMaxGasAmount)
{
    maxGasAmount = newMaxGasAmount;
}

const std::string& TransferCoinBCSResponse::getGasUnitPrice() const
{
    return gasUnitPrice;
}

void TransferCoinBCSResponse::setGasUnitPrice(const std::string& newGasUnitPrice)
{
    gasUnitPrice = newGasUnitPrice;
}

const std::string& TransferCoinBCSResponse::getExpirationTimestampSecs() const
{
    return expirationTimestampSecs;
}

void TransferCoinBCSResponse::setExpirationTimestampSecs(const std::string& newExpirationTimestampSecs)
{
    expirationTimestampSecs = newExpirationTimestampSecs;
}

nlohmann::json TransferCoinBCSResponse::to_json() const
{
    return {
        {"hash", hash},
        {"sender", sender},
        {"sequence_number", sequenceNumber},
        {"max_gas_amount", maxGasAmount},
        {"gas_unit_price", gasUnitPrice},
        {"expiration_timestamp_secs", expirationTimestampSecs}
    };
}

TransferCoinBCSResponse TransferCoinBCSResponse::from_json(const nlohmann::json& j)
{
    TransferCoinBCSResponse response;
    response.setHash(j.value("hash", ""));
    response.setSender(j.value("sender", ""));
    response.setSequenceNumber(j.value("sequence_number", ""));
    response.setMaxGasAmount(j.value("max_gas_amount", ""));
    response.setGasUnitPrice(j.value("gas_unit_price", ""));
    response.setExpirationTimestampSecs(j.value("expiration_timestamp_secs", ""));
    return response;
}
}
