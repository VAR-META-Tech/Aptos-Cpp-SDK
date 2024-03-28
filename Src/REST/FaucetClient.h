#ifndef FAUCETCLIENT_H
#define FAUCETCLIENT_H

#include <functional>
#include <string>
#include "Model/ResponseInfo.h"

namespace Aptos::Rest
{
    /// <summary>
    /// Faucet Client for claiming APT from Devnet.
    /// To claim APT from Testnet you can visit Aptos Testnet Airdrop site.
    /// </summary>
    class APTOS_API FaucetClient
    {
    public:
        /// <summary>
        /// Funds a Testnet Account
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="address">Address that will get funded.</param>
        /// <param name="amount">Amount of APT requested.</param>
        /// <param name="endpoint">Base URL for faucet.</param>
        /// <returns>Calls <c>callback</c> function with <c>(bool, ResponsiveInfo)</c>: \n
        /// A boolean stating that the request for funding was successful, and an object containg the response details</returns>
        static void FundAccount(std::function<void(bool, AptosRESTModel::ResponseInfo)> callback,
                                const std::string &address, int amount, std::string &endpoint);
    };

}
#endif // FAUCETCLIENT_H
