#ifndef FAUCETCLIENT_H
#define FAUCETCLIENT_H

#include <functional>
#include <string>
#include <cpprest/http_client.h>
#include "Model/ResponseInfo.h"

class FaucetClient {
public:
    static void FundAccount(std::function<void(bool, AptosRESTModel::ResponseInfo)> callback,
                            const std::string& address, int amount, const std::string& endpoint);};


#endif // FAUCETCLIENT_H
