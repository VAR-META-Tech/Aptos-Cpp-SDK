#ifndef RESTTRANSACTIONPAYLOAD_H
#define RESTTRANSACTIONPAYLOAD_H

#include <nlohmann/json.hpp>
#include <vector>
namespace AptosRESTModel {

class Arguments {
public:
    nlohmann::json ToJson() const;

    static Arguments FromJson(const nlohmann::json& argsJson);
private:
    std::vector<std::string> ArgumentStrings;
    std::vector<bool> MutateSettings;
    std::vector<std::string> PropertyKeys;
    std::vector<int> PropertyValues;
    std::vector<std::string> PropertyTypes;
};

/// <summary>
/// Represents Transaction Payload
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/TransactionPayload
/// NOTE: Arguments is serialized to a single object that contains arrays of different types
/// </summary>
class TransactionPayload {
public:
    nlohmann::json ToJson() const;
    static TransactionPayload FromJson(const nlohmann::json& payloadJson);
private:
    std::string Type;
    std::string Function;
    std::vector<std::string> TypeArguments;
    Arguments ArgumentsProp;
};
}


#endif // RESTTRANSACTIONPAYLOAD_H
