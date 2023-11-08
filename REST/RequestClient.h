#ifndef REQUESTCLIENT_H
#define REQUESTCLIENT_H

#include <cpprest/http_client.h>
#include <cpprest/uri.h>

class RequestClient {
public:
    static web::http::http_request SubmitRequest(web::http::uri uri, web::http::method method = web::http::methods::GET);
    static utility::string_t GetAptosHeaderValue();
};


#endif // REQUESTCLIENT_H
