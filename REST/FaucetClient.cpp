#include "FaucetClient.h"
#include <iostream>
#include <thread>

namespace Aptos::Rest
{
    void FaucetClient::FundAccount(std::function<void(bool, AptosRESTModel::ResponseInfo)> callback,
                                   const std::string &address, int amount, const std::string &endpoint)
    {
        // using namespace AptosRESTModel;
        // std::string faucetURL = endpoint + "/mint?amount=" + std::to_string(amount) + "&address=" + address;
        // web::http::client::http_client client(faucetURL);

        // web::http::http_request request(web::http::methods::POST);
        // request.headers().set_content_type(U("application/json"));

        // client.request(request).then([callback](web::http::http_response response)
        //                              {
        //                        ResponseInfo responseInfo;
        //                        if (response.status_code() == web::http::status_codes::NotFound) {
        //                            responseInfo.status = ResponseInfo::Status::NotFound;
        //                            responseInfo.message = "Not Found";
        //                            callback(false, responseInfo);
        //                        } else if (response.status_code() >= 400) {
        //                            responseInfo.status = ResponseInfo::Status::Failed;
        //                            responseInfo.message = "Request Failed";
        //                            callback(false, responseInfo);
        //                        } else {
        //                            responseInfo.status = ResponseInfo::Status::Success;
        //                            responseInfo.message = "Funding succeeded!";
        //                            std::this_thread::sleep_for(std::chrono::seconds(2));
        //                            callback(true, responseInfo);
        //                        } })
        //     .wait();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
