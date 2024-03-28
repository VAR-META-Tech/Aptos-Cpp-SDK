#ifndef ACCOUNTDATA_H
#define ACCOUNTDATA_H

#include <string>
#include <nlohmann/json.hpp>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {
/// <summary>
/// Represents Account Data object
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/AccountData
/// </summary>
class APTOS_API AccountData {
public:
    explicit AccountData() = default;

    explicit AccountData(const std::string& sequenceNumber, const std::string& authenticationKey);
    // Serialize the object to JSON
    nlohmann::json ToJson() const;
    // Deserialize the JSON to an object
    static AccountData FromJson(const nlohmann::json& jsonObject);
    const std::string &getSequenceNumber() const;

    std::string getAuthenticationKey() const;
    void setAuthenticationKey(const std::string &newAuthenticationKey);

private:
    std::string SequenceNumber;
    std::string AuthenticationKey;
};
}
#endif // ACCOUNTDATA_H
