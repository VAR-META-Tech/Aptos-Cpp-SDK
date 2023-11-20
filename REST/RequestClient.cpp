#include "RequestClient.h"
#include <iostream>
#include <cpprest/json.h>

namespace Aptos::Rest
{
    web::http::http_request RequestClient::SubmitRequest(web::uri uri, web::http::method method)
    {
        web::http::http_request request(method);
        request.headers().add(U("x-aptos-client"), GetAptosHeaderValue());
        request.set_request_uri(uri);
        return request;
    }

    utility::string_t RequestClient::GetAptosHeaderValue()
    {
        return U("aptos-cpp-sdk/") + utility::conversions::to_string_t("0.0.1");
    }
}