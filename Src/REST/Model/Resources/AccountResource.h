#ifndef ACCOUNTRESOURCE_H
#define ACCOUNTRESOURCE_H

#include <nlohmann/json.hpp>
#include "Base/ResourceEvent.h"
#include "Base/CapabilityOffer.h"
namespace AptosRESTModel {

class AccountResourceData {
public:
    nlohmann::json ToJson() const;
    static AccountResourceData FromJson(const nlohmann::json& dataJson);
    std::string getGuidCreationNum() const;
    void setGuidCreationNum(const std::string &newGuidCreationNum);

private:
    std::string AuthenticationKey;
    ResourceEvent CoinRegisterEvents;
    std::string GuidCreationNum;
    ResourceEvent KeyRotationEvents;
    CapabilityOffer RotationCapabilityOffer;
    std::string SequenceNumber;
    CapabilityOffer SignerCapabilityOffer;
};

class AccountResource {
public:
    nlohmann::json ToJson() const;
    static AccountResource FromJson(const nlohmann::json& resourceJson);

    AccountResourceData getData() const;
    void setData(const AccountResourceData &newData);

private:
    AccountResourceData Data;
};
}
#endif // ACCOUNTRESOURCE_H
