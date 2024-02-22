#include "TransactionPayload.h"

namespace AptosRESTModel {

nlohmann::json TransactionPayload::ToJson() const {
    nlohmann::json payloadJson;
    payloadJson["type"] = Type;
    payloadJson["function"] = Function;
    payloadJson["type_arguments"] = TypeArguments;
    payloadJson["arguments"] = ArgumentsProp.ToJson();
    return payloadJson;
}

TransactionPayload TransactionPayload::FromJson(const nlohmann::json &payloadJson) {
    TransactionPayload payload;
    payload.Type = payloadJson["type"];
    payload.Function = payloadJson["function"];
    payload.TypeArguments = payloadJson["type_arguments"].get<std::vector<std::string>>();
    payload.ArgumentsProp = Arguments::FromJson(payloadJson["arguments"]);
    return payload;
}

std::string TransactionPayload::getType() const
{
    return Type;
}

void TransactionPayload::setType(const std::string &newType)
{
    Type = newType;
}

std::string TransactionPayload::getFunction() const
{
    return Function;
}

void TransactionPayload::setFunction(const std::string &newFunction)
{
    Function = newFunction;
}

std::vector<std::string> TransactionPayload::getTypeArguments() const
{
    return TypeArguments;
}

void TransactionPayload::setTypeArguments(const std::vector<std::string> &newTypeArguments)
{
    TypeArguments = newTypeArguments;
}

Arguments TransactionPayload::getArgumentsProp() const
{
    return ArgumentsProp;
}

void TransactionPayload::setArgumentsProp(const Arguments &newArgumentsProp)
{
    ArgumentsProp = newArgumentsProp;
}

nlohmann::json Arguments::ToJson() const {
    nlohmann::json argsJson;
    argsJson["argument_strings"] = ArgumentStrings;
    argsJson["mutate_settings"] = MutateSettings;
    argsJson["property_keys"] = PropertyKeys;
    argsJson["property_values"] = PropertyValues;
    argsJson["property_types"] = PropertyTypes;
    return argsJson;
}

Arguments Arguments::FromJson(const nlohmann::json &argsJson) {
    Arguments args;
    args.ArgumentStrings = argsJson["argument_strings"].get<std::vector<std::string>>();
    args.MutateSettings = argsJson["mutate_settings"].get<std::vector<bool>>();
    args.PropertyKeys = argsJson["property_keys"].get<std::vector<std::string>>();
    args.PropertyValues = argsJson["property_values"].get<std::vector<int>>();
    args.PropertyTypes = argsJson["property_types"].get<std::vector<std::string>>();
    return args;
}

std::vector<std::string> Arguments::getArgumentStrings() const
{
    return ArgumentStrings;
}

void Arguments::setArgumentStrings(const std::vector<std::string> &newArgumentStrings)
{
    ArgumentStrings = newArgumentStrings;
}

std::vector<bool> Arguments::getMutateSettings() const
{
    return MutateSettings;
}

void Arguments::setMutateSettings(const std::vector<bool> &newMutateSettings)
{
    MutateSettings = newMutateSettings;
}

}
