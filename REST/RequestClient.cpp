#include "RequestClient.h"
#include <iostream>

namespace Aptos::Rest
{
    httplib::Client RequestClient::GetWebClient(std::string &uri)
    {
        httplib::Client cli(uri);
        cli.enable_server_certificate_verification(false);
        return cli;
    }

    std::string RequestClient::GetAptosHeaderValue()
    {
        return "aptos-cpp-sdk/0.0.1";
    }
}
