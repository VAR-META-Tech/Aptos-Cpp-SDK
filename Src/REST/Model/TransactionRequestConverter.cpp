#include "TransactionRequestConverter.h"
#include "../Constant.h"

using namespace Aptos::BCS;
namespace AptosRESTModel {

AptosRESTModel::TransactionRequest TransactionRequestConverter::ReadJson(const nlohmann::json &jsonData)
{
    AptosRESTModel::TransactionRequest transactionRequest;
    AptosRESTModel::TransactionPayload transactionPayload;
    // STEP 1: Parse through default properties
    if (!jsonData["sender"].is_null()) {
        transactionRequest.setSender(jsonData["sender"]);
    }
    if (!jsonData["sequence_number"].is_null())
        transactionRequest.setSequenceNumber(jsonData["sequence_number"]);

    if (!jsonData["max_gas_amount"].is_null())
        transactionRequest.setMaxGasAmount(jsonData["max_gas_amount"]);

    if (!jsonData["gas_unit_price"].is_null())
        transactionRequest.setGasUnitPrice(jsonData["gas_unit_price"]);

    if (!jsonData["expiration_timestamp_secs"].is_null())
        transactionRequest.setExpirationTimestampSecs(jsonData["expiration_timestamp_secs"]);

    // STEP 2: Parse through default Payload arguments
    if (!jsonData["payload"].is_null()) {
        nlohmann::json payloadData = jsonData["payload"];
        transactionPayload.setFunction(payloadData["function"]);
        transactionPayload.setType(payloadData["type"]);
        if (!payloadData["type_arguments"].is_null()) {
            transactionPayload.setTypeArguments(payloadData["type_arguments"]);
        }


    }
    // STEP 3: Parse Payload object
    if (!jsonData["payload"].is_null() && !jsonData["payload"]["arguments"].is_null())
    {
        nlohmann::json arguments = jsonData["payload"]["arguments"];
        std::vector<std::string> argumentStrings;
        std::vector<bool> argumentBooleans;

        for (const auto &arg : arguments)
        {
            if (arg.is_string())
            {
                std::string argStr = arg.get<std::string>();
                argumentStrings.push_back(argStr);
            }

            if (arg.is_array())
            {
                nlohmann::json arrayVal = arg;
                for (const auto &argument : arrayVal)
                {
                    if (argument.is_boolean())
                    {
                        argumentBooleans.push_back(argument.get<bool>());
                    }

                    if (argument.is_string())
                    {
                        argumentStrings.push_back(argument.get<std::string>());
                    }
                }
            }
        }
        AptosRESTModel::Arguments arg;
        arg.setArgumentStrings(argumentStrings);
        arg.setMutateSettings(argumentBooleans);
        transactionPayload.setArgumentsProp(arg);
    }
    // STEP 4: Parse Signature
    if (!jsonData["signature"].is_null()) {
        nlohmann::json signatureData = jsonData["signature"];
        AptosRESTModel::SignatureData signature;

        signature.setType(signatureData["type"]);
        if (signature.getType() != "multi_ed25519_signature"){
            signature.setPublicKey(signatureData["public_key"]);
            signature.setSignature(signatureData["signature"]);
        }
        transactionRequest.setSignature(signature);
    }
    return transactionRequest;
}

nlohmann::json TransactionRequestConverter::WriteJson(const AptosRESTModel::TransactionRequest &transactionRequest)
{
    nlohmann::json oTransactionRequest;

    oTransactionRequest["sender"] = transactionRequest.getSender();
    oTransactionRequest["sequence_number"] = transactionRequest.getSequenceNumber();
    oTransactionRequest["max_gas_amount"] = transactionRequest.getMaxGasAmount();
    oTransactionRequest["gas_unit_price"] = transactionRequest.getGasUnitPrice();
    oTransactionRequest["expiration_timestamp_secs"] = transactionRequest.getExpirationTimestampSecs();

    // PAYLOAD
    Aptos::BCS::EntryFunction entryFunction = transactionRequest.getEntryFunction();

    nlohmann::json oPayload;
    oPayload["function"] = entryFunction.getModule().ToString() + "::" + entryFunction.getFunction();

    nlohmann::json jTypeArguments = nlohmann::json::array();;
    Aptos::BCS::TagSequence tagSeq = entryFunction.getTypeArgs();
    std::vector<std::shared_ptr<ISerializableTag>> typeArgs = tagSeq.GetValue();

    for (const auto& typeArg : typeArgs)
    {
        jTypeArguments.push_back(typeArg->ToString());
    }
    oPayload["type_arguments"] = jTypeArguments;

    nlohmann::json jArguments;
    std::vector<std::shared_ptr<ISerializable>> args = transactionRequest.getEntryFunction().getArgs().GetValue();
    for (const auto& arg : args)
    {
        jArguments.push_back(arg->ToString());
    }

    oPayload["arguments"] = jArguments;
    oPayload["type"] = Constants::ENTRY_FUNCTION_PAYLOAD;

    oTransactionRequest["payload"] = oPayload;

    // SIGNATURE
    if (!transactionRequest.getSignature().getType().empty())
    {
    nlohmann::json signature = transactionRequest.getSignature().ToJson();
    oTransactionRequest["signature"] = signature;
    }

    return oTransactionRequest;
}
}
