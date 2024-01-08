#include "AddTokenPropertyResponse.h"

nlohmann::json AptosSDKDemo::AddTokenPropertyResponse::to_json() const
{
    nlohmann::json jsonObject;
    jsonObject["hash"] = Hash;
    jsonObject["sender"] = Sender;
    return jsonObject;
}

AptosSDKDemo::AddTokenPropertyResponse AptosSDKDemo::AddTokenPropertyResponse::from_json(const nlohmann::json &j)
{
    AddTokenPropertyResponse response;
    response.Hash = j.value("hash", "");
    response.Sender = j.value("sender", "");

    return response;
}


namespace AptosSDKDemo {
std::string AddTokenPropertyResponse::getHash() const
{
    return Hash;
}

void AddTokenPropertyResponse::setHash(const std::string &newHash)
{
    Hash = newHash;
}

}
