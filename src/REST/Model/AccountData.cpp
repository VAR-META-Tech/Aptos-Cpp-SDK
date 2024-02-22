#include "AccountData.h"
#include "iostream"

namespace AptosRESTModel {

AccountData::AccountData(const std::string &sequenceNumber, const std::string &authenticationKey)
    : SequenceNumber(sequenceNumber), AuthenticationKey(authenticationKey) {}

nlohmann::json AccountData::ToJson() const {
    nlohmann::json jsonObject;
    jsonObject["sequence_number"] = SequenceNumber;
    jsonObject["authentication_key"] = AuthenticationKey;
    return jsonObject;
}

AccountData AccountData::FromJson(const nlohmann::json &jsonObject) {
    AccountData accountData;
    accountData.SequenceNumber = jsonObject["sequence_number"];
    accountData.AuthenticationKey = jsonObject["authentication_key"];
    return accountData;
}

const std::string &AccountData::getSequenceNumber() const
{
    return SequenceNumber;
}

std::string AccountData::getAuthenticationKey() const
{
    return AuthenticationKey;
}

void AccountData::setAuthenticationKey(const std::string &newAuthenticationKey)
{
    AuthenticationKey = newAuthenticationKey;
}
}
