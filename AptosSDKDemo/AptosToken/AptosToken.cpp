#include "AptosToken.h"
#include <iostream>
#include <string>
#include "../../REST/FaucetClient.h"
#include "../../REST/RestClient.h"
#include "../../REST/Constant.h"
namespace AptosSDKDemo {

AptosToken::AptosToken() {}

void AptosToken::Start()
{
    std::cout << "<color=cyan>=== =========================== ===</color>" << std::endl;
    std::cout << "<color=cyan>=== Set Up Faucet & REST Client ===</color>" << std::endl;
    std::cout << "<color=cyan>=== =========================== ===</color>" << std::endl;

    std::string faucetEndpoint = "https://faucet.devnet.aptoslabs.com";

    Aptos::Rest::FaucetClient faucetClient;
    Aptos::Rest::RestClient restClient(Constants::DEVNET_BASE_URL);
    // Note: The equivalent of 'yield return' depends on the coroutine implementation.

    // AptosTokenClient* tokenClient = AptosTokenClient::Instance()->SetUp(restClient);

}

}
