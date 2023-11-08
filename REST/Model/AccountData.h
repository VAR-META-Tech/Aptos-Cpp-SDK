#ifndef ACCOUNTDATA_H
#define ACCOUNTDATA_H

#include <string>
#include <nlohmann/json.hpp>

namespace AptosRESTModel {
/// <summary>
/// Represents Account Data object
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/AccountData
/// </summary>
class AccountData {
public:
    AccountData();

    AccountData(const std::string& sequenceNumber, const std::string& authenticationKey);
    // Serialize the object to JSON
    nlohmann::json ToJson() const;
    // Deserialize the JSON to an object
    static AccountData FromJson(const nlohmann::json& jsonObject);
private:
    std::string SequenceNumber;
    std::string AuthenticationKey;
};
}
#endif // ACCOUNTDATA_H
