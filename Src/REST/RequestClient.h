#ifndef REQUESTCLIENT_H
#define REQUESTCLIENT_H

#include <cpprest/http_client.h>
#include <cpprest/uri.h>
#include "BCS/BCSTypes.h"

namespace Aptos::Rest
{
    /// <summary>
    /// UnityWebRequest wrapper client
    /// </summary>
    class APTOS_API RequestClient
    {
    public:
        /// <summary>
        /// Get the UnityWebRequest object for the given path, default to GET method
        /// </summary>
        /// <param name="uri">endpoint uri</param>
        /// <param name="method">HTTP method</param>
        /// <returns>UnityWebRequest object</returns>
        static web::http::http_request SubmitRequest(web::http::uri uri, web::http::method method = web::http::methods::GET);

        /// <summary>
        /// Get the default Aptos header value
        /// </summary>
        /// <returns>String with the default Aptos header value</returns>
        static utility::string_t GetAptosHeaderValue();
    };
}
#endif // REQUESTCLIENT_H