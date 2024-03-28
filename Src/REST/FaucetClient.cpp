#include "FaucetClient.h"
#include <iostream>
#include <thread>
#include <cpprest/http_client.h>
#include <cpprest/json.h>

namespace Aptos::Rest
{
    void FaucetClient::FundAccount(std::function<void(bool, AptosRESTModel::ResponseInfo)> callback,
                                   const std::string &address, int amount, std::string &endpoint)
    {
        using namespace AptosRESTModel;
        std::string uri = endpoint + "/mint?amount=" + std::to_string(amount) + "&address=" + address;
        web::http::client::http_client client(utility::conversions::to_string_t(uri));
        web::http::http_request request(web::http::methods::POST);
        request.headers().set_content_type(U("application/json"));
        client.request(request).then([callback](web::http::http_response response)
        {
            ResponseInfo responseInfo;
            if (response.status_code() >= 400)
            {
                responseInfo.status = ResponseInfo::Status::Failed;
                responseInfo.message = "Request Failed";
                callback(false, responseInfo);
            }
            else
            {
                responseInfo.status = ResponseInfo::Status::Success;
                responseInfo.message = "Funding succeeded!";
                callback(true, responseInfo);
            }
        } ).wait();
    }
}
