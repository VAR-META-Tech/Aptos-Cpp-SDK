#include "AccountData.h"
#include "iostream"

namespace AptosRESTModel {

AccountData::AccountData()
{

}

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
}
