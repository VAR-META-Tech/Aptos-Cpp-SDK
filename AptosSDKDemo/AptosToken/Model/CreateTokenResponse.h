#ifndef CREATETOKENRESPONSE_H
#define CREATETOKENRESPONSE_H
#include <nlohmann/json.hpp>

namespace AptosSDKDemo {

class CreateTokenResponse
{
public:
    nlohmann::json to_json() const;

    static CreateTokenResponse from_json(const nlohmann::json& j);
    std::string getHash() const;
    void setHash(const std::string &newHash);

    std::string getSender() const;
    void setSender(const std::string &newSender);

private:
    std::string Hash;
    std::string Sender;
};
}
#endif // CREATETOKENRESPONSE_H
