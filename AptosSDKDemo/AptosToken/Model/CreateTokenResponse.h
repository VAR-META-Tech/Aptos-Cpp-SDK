#ifndef CREATETOKENRESPONSE_H
#define CREATETOKENRESPONSE_H
#include <nlohmann/json.hpp>

namespace AptosSDKDemo {

class CreateTokenResponse
{
public:
    nlohmann::json to_json() const;

    static CreateTokenResponse from_json(const nlohmann::json& j);
private:
    std::string Hash;
    std::string Sender;
};
}
#endif // CREATETOKENRESPONSE_H
