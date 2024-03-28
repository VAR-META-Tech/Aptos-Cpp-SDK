#ifndef RESTTRANSACTIONPAYLOAD_H
#define RESTTRANSACTIONPAYLOAD_H

#include <nlohmann/json.hpp>
#include <vector>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {

class APTOS_API Arguments {
public:
    nlohmann::json ToJson() const;

    static Arguments FromJson(const nlohmann::json& argsJson);
    std::vector<std::string> getArgumentStrings() const;
    void setArgumentStrings(const std::vector<std::string> &newArgumentStrings);

    std::vector<bool> getMutateSettings() const;
    void setMutateSettings(const std::vector<bool> &newMutateSettings);

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
class APTOS_API TransactionPayload {
public:
    nlohmann::json ToJson() const;
    static TransactionPayload FromJson(const nlohmann::json& payloadJson);
    std::string getType() const;
    void setType(const std::string &newType);

    std::string getFunction() const;
    void setFunction(const std::string &newFunction);

    std::vector<std::string> getTypeArguments() const;
    void setTypeArguments(const std::vector<std::string> &newTypeArguments);

    Arguments getArgumentsProp() const;
    void setArgumentsProp(const Arguments &newArgumentsProp);

private:
    std::string Type;
    std::string Function;
    std::vector<std::string> TypeArguments;
    Arguments ArgumentsProp;
};
}


#endif // RESTTRANSACTIONPAYLOAD_H
