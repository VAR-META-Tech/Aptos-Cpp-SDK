#ifndef REQUESTCLIENT_H
#define REQUESTCLIENT_H
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

namespace Aptos::Rest
{
    /// <summary>
    /// UnityWebRequest wrapper client
    /// </summary>
    class RequestClient
    {
    public:
        /// <summary>
        /// Get the Client object for the given path, default to GET method
        /// </summary>
        /// <param name="uri">endpoint uri</param>
        /// <returns>httplib::Client object</returns>
        static httplib::Client GetWebClient(std::string uri);

        /// <summary>
        /// Get the default Aptos header value
        /// </summary>
        /// <returns>String with the default Aptos header value</returns>
        static std::string GetAptosHeaderValue();
    };
}
#endif // REQUESTCLIENT_H
