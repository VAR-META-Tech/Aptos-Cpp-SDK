#include "TransactionRequest.h"
namespace AptosRESTModel {

nlohmann::json TransactionRequest::ToJson() const {
    nlohmann::json requestJson;
    requestJson["sender"] = Sender;
    requestJson["sequence_number"] = SequenceNumber;
    requestJson["max_gas_amount"] = MaxGasAmount;
    requestJson["gas_unit_price"] = GasUnitPrice;
    requestJson["expiration_timestamp_secs"] = ExpirationTimestampSecs;
    requestJson["payload"] = Payload.ToJson();
//    requestJson["entryFunction"] = EntryFunction.ToJson();
    if (!Signature.getType().empty() || !Signature.getPublicKey().empty() || !Signature.getSignature().empty()) {
        requestJson["signature"] = Signature.ToJson();
    }
    return requestJson;
}

TransactionRequest TransactionRequest::FromJson(const nlohmann::json &requestJson) {
    TransactionRequest request;
    request.Sender = requestJson["sender"];
    request.SequenceNumber = requestJson["sequence_number"];
    request.MaxGasAmount = requestJson["max_gas_amount"];
    request.GasUnitPrice = requestJson["gas_unit_price"];
    request.ExpirationTimestampSecs = requestJson["expiration_timestamp_secs"];
    request.Payload = TransactionPayload::FromJson(requestJson["payload"]);
//    request.EntryFunction = EntryFunction::FromJson(requestJson["entryFunction"]);
    if (requestJson.find("signature") != requestJson.end()) {
        request.Signature = SignatureData::FromJson(requestJson["signature"]);
    }
    return request;
}

nlohmann::json SignatureData::ToJson() const {
    nlohmann::json signatureJson;
    signatureJson["type"] = Type;
    signatureJson["public_key"] = PublicKey;
    signatureJson["signature"] = Signature;
    return signatureJson;
}

SignatureData SignatureData::FromJson(const nlohmann::json &signatureJson) {
    SignatureData signature;
    signature.Type = signatureJson["type"];
    signature.PublicKey = signatureJson["public_key"];
    signature.Signature = signatureJson["signature"];
    return signature;
}

std::string SignatureData::getType() const
{
    return Type;
}

std::string SignatureData::getPublicKey() const
{
    return PublicKey;
}

std::string SignatureData::getSignature() const
{
    return Signature;
}

}
