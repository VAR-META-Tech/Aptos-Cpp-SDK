#include "FaucetClient.h"
#include <iostream>
#include <thread>
#include "RequestClient.h"

namespace Aptos::Rest
{
    void FaucetClient::FundAccount(std::function<void(bool, AptosRESTModel::ResponseInfo)> callback,
                                   const std::string &address, int amount, const std::string &endpoint)
    {
        using namespace AptosRESTModel;
        std::string uri = "/mint?amount=" + std::to_string(amount) + "&address=" + address;
        ResponseInfo responseInfo;
        if (auto response = RequestClient::GetWebClient(endpoint).Post(uri, "", "application/json"))
        {
            if (response->status >= 400)
            {
                responseInfo.status = ResponseInfo::Status::Failed;
                responseInfo.message = "Request Failed";
                callback(false, responseInfo);
            }
            else
            {
                responseInfo.status = ResponseInfo::Status::Success;
                responseInfo.message = "Funding succeeded!";
                std::this_thread::sleep_for(std::chrono::seconds(2));
                callback(true, responseInfo);
            }
        } else {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = "Request Failed";
            callback(false, responseInfo);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
