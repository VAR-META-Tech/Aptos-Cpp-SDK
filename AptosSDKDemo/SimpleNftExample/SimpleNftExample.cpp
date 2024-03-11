#include "SimpleNftExample.h"
#include <iostream>
#include <string>
#include "REST/RestClient.h"
#include "REST/FaucetClient.h"
#include "REST/Constant.h"

using namespace Aptos::Accounts;
using namespace Aptos::BCS;
namespace AptosSDKDemo {

SimpleNftExample::SimpleNftExample() {
    std::cout << "=== =========================== ===" << std::endl;
    std::cout << "=== SimpleNftExample            ===" << std::endl;
    std::cout << "=== =========================== ===" << std::endl;
}

// This code demonstrates basic NFT (Non-Fungible Token) operations on the Aptos blockchain using the Devnet:
//
// * **Setup:**  Creates accounts for Alice and Bob, funds them using the Devnet faucet.
// * **Collection Creation:**  Alice creates an NFT collection.
// * **Token Minting:**  Alice mints a new NFT within her collection.
// * **Token Transfer:**  Alice offers the NFT to Bob, and Bob claims it.
// * **Lookup:** Demonstrates looking up token balances and metadata. 
void SimpleNftExample::Start()
{
    Account alice;
    Account bob;

    std::cout << "..=== Addresses ===.." << std::endl;
    std::cout << "Alice: " << alice.getAccountAddress()->ToString() << std::endl;
    std::cout << "Bob: " << bob.getAccountAddress()->ToString() << std::endl;

    // REST & Faucet Client Setup
    std::string faucetEndpoint = "https://faucet.devnet.aptoslabs.com";
    Aptos::Rest::FaucetClient faucetClient;
    Aptos::Rest::RestClient restClient(Constants::DEVNET_BASE_URL);

    auto aliceAddress = alice.getAccountAddress();

    bool success = false;
    AptosRESTModel::ResponseInfo responseInfo;
    faucetClient.FundAccount([&](bool _success, const AptosRESTModel::ResponseInfo& _responseInfo)
                             {
                                 success = _success;
                                 responseInfo = _responseInfo;
                             }, aliceAddress->ToString(), 100000000, faucetEndpoint);

    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success)
    {
        std::cerr << "Faucet funding for Alice failed: " << responseInfo.message << std::endl;
        return;
    }

    // Fund Bob Account Through Devnet Faucet
    auto bobAddress = bob.getAccountAddress();

    success = false;
    responseInfo = AptosRESTModel::ResponseInfo();
    faucetClient.FundAccount([&](bool _success, const AptosRESTModel::ResponseInfo& _responseInfo)
                             {
                                 success = _success;
                                 responseInfo = _responseInfo;
                             }, bobAddress->ToString(), 100000000, faucetEndpoint);

    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success)
    {
        std::cerr << "Faucet funding for Bob failed: " << responseInfo.message << std::endl;
        return;
    }

    //Initial Coin Balances
    AptosRESTModel::AccountResourceCoin::Coin coin;
    restClient.GetAccountBalance([&](const AptosRESTModel::AccountResourceCoin::Coin& _coin, const AptosRESTModel::ResponseInfo& _responseInfo)
                                 {
                                     coin = _coin;
                                     responseInfo = _responseInfo;
                                 }, *aliceAddress);

    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed)
    {
        std::cerr << responseInfo.message << std::endl;
        return;
    }

    std::cout << "Alice's Balance After Funding: " << coin.value() << std::endl;

    // Lookup Bob's balance using a view function
    std::vector<std::string> data;
    AptosRESTModel::ViewRequest viewRequest;
    viewRequest.setFunction("0x1::coin::balance");
    viewRequest.setTypeArguments({ "0x1::aptos_coin::AptosCoin" });
    viewRequest.setArguments({ bobAddress->ToString() });
    restClient.View([&](const std::vector<std::string>& _data, const AptosRESTModel::ResponseInfo& _responseInfo)
                    {
                        data = _data;
                        responseInfo = _responseInfo;
                    }, viewRequest);

    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed)
    {
        std::cerr << responseInfo.message << std::endl;
        return;
    }

    std::cout << "Bob's Balance After Funding: " << std::stoull(data[0]) << std::endl;

    // Collection & Token Naming Details
    std::string collectionName = "Alice's";
    std::string collectionDescription = "Alice's simple collection";
    std::string collectionUri = "https://aptos.dev";

    std::string tokenName = "Alice's first token";
    std::string tokenDescription = "Alice's simple token";
    std::string tokenUri = "https://aptos.dev/img/nyan.jpeg";
    int propertyVersion = 0;
    // Create Collection
    std::cout << "..=== Creating Collection and Token ===.." << std::endl;
    AptosRESTModel::Transaction createCollectionTxn;
    restClient.CreateCollection([&](const AptosRESTModel::Transaction& _createCollectionTxn, const AptosRESTModel::ResponseInfo& _responseInfo)
                                {
                                    createCollectionTxn = _createCollectionTxn;
                                    responseInfo = _responseInfo;
                                }, alice, collectionName, collectionDescription, collectionUri);

    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success)
    {
        std::cerr << "Cannot create collection. " << responseInfo.message << std::endl;
    }

    std::cout << "Create Collection Response: " << responseInfo.message << std::endl;
    std::string transactionHash = createCollectionTxn.getHash();
    std::cout << "Create Collection Hash: " << createCollectionTxn.getHash() << std::endl;

    // Wait For Transaction
    bool waitForTxnSuccess = false;
    restClient.WaitForTransaction([&](bool _pending, const AptosRESTModel::ResponseInfo& _responseInfo)
                                  {
                                      waitForTxnSuccess = _pending;
                                      responseInfo = _responseInfo;
                                  }, transactionHash);

    if (!waitForTxnSuccess)
    {
        std::cout << "Transaction was not found. Breaking out of example: Error: " << responseInfo.message << std::endl;
        return;
    }

    // Create Non-Fungible Token
    AptosRESTModel::Transaction createTokenTxn;
    restClient.CreateToken([&](const AptosRESTModel::Transaction& _createTokenTxn, const AptosRESTModel::ResponseInfo& _responseInfo)
                           {
                               createTokenTxn = _createTokenTxn;
                               responseInfo = _responseInfo;
                           }, alice, collectionName, tokenName, tokenDescription, 1, 1, tokenUri, 0);

    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success)
    {
        std::cout << "Error creating token. " << responseInfo.message << std::endl;
    }

    std::cout << "Create Token Response: " << responseInfo.message << std::endl;
    std::cout << "Create Token Hash: " << createTokenTxn.getHash() << std::endl;
    restClient.WaitForTransaction([&](bool _pending, const AptosRESTModel::ResponseInfo& _responseInfo)
                                  {
                                      waitForTxnSuccess = _pending;
                                      responseInfo = _responseInfo;
                                  }, transactionHash);

    if (!waitForTxnSuccess)
    {
        std::cout << "Transaction was not found. Breaking out of example: Error: " << responseInfo.message << std::endl;
        return;
    }
    std::string getCollectionResult = "";
    restClient.GetCollection([&](const std::string& returnResult)
                             {
                                 getCollectionResult = returnResult;
                             }, *aliceAddress, collectionName);
    std::cout << "Alice's Collection: " << getCollectionResult << std::endl;

    //Get Token Balance
    std::string getTokenBalanceResultAlice = "";
    restClient.GetTokenBalance([&](const std::string& returnResult)
                               {
                                   getTokenBalanceResultAlice = returnResult;
                               }, *aliceAddress, *aliceAddress, collectionName, tokenName, propertyVersion);
    std::cout << "Alice's NFT Token Balance: " << getTokenBalanceResultAlice << std::endl;

    std::shared_ptr<AptosRESTModel::TableItemTokenMetadata> tableItemToken;
    restClient.GetTokenData([&](std::shared_ptr<AptosRESTModel::TableItemTokenMetadata> _tableItemToken, const AptosRESTModel::ResponseInfo& _responseInfo)
                            {
                                // getTokenDataResultAlice = returnResult;
                                tableItemToken = _tableItemToken;
                                responseInfo = _responseInfo;
                            }, *aliceAddress, collectionName, tokenName, propertyVersion);

    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success)
    {
        std::cerr << "Could not get token data." << std::endl;
        return;
    }

    std::cout << "Alice's Token Data: " << tableItemToken->ToJson() << std::endl;

    // Transferring the Token to Bob
    std::cout << "..=== Alice Offering Token to Bob ===.." << std::endl;
    std::shared_ptr<AptosRESTModel::Transaction> offerTokenTxn;
    restClient.OfferToken([&](std::shared_ptr<AptosRESTModel::Transaction> _offerTokenTxn, const AptosRESTModel::ResponseInfo& _responseInfo)
                          {
                              offerTokenTxn = _offerTokenTxn;
                              responseInfo = _responseInfo;
                          }, alice, *bob.getAccountAddress(), *alice.getAccountAddress(), collectionName, tokenName, 1);


    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success)
    {
        std::cerr << "Error offering token. " << responseInfo.message << std::endl;
        return;
    }

    std::cout << "Offer Token Response: " << responseInfo.message << std::endl;
    std::cout << "Offer Sender: " << offerTokenTxn->getSender() << std::endl;
    std::string offerTokenTxnHash = offerTokenTxn->getHash();
    std::cout << "Offer Token Hash: " << offerTokenTxnHash << std::endl;
    restClient.WaitForTransaction([=](bool pending, const AptosRESTModel::ResponseInfo& _responseInfo)
                                  {
                                      std::cout << _responseInfo.message << std::endl;
                                  }, offerTokenTxnHash);

    // Bob Claims Token
    std::cout << "..=== Bob Claims Token ===.." << std::endl;

    std::shared_ptr<AptosRESTModel::Transaction> claimTokenTxn;
    restClient.ClaimToken([&](std::shared_ptr<AptosRESTModel::Transaction> _claimTokenTxn, const AptosRESTModel::ResponseInfo& _responseInfo)
                          {
                              claimTokenTxn = _claimTokenTxn;
                              responseInfo = _responseInfo;
                          }, bob, *alice.getAccountAddress(), *alice.getAccountAddress(), collectionName, tokenName, propertyVersion);


    std::cout << "Claim Token Response: " << responseInfo.message << std::endl;
    std::string claimTokenTxnHash = claimTokenTxn->getHash();
    std::cout << "Claim Token Hash: " << claimTokenTxnHash << std::endl;

    restClient.WaitForTransaction([=](bool pending, const AptosRESTModel::ResponseInfo& _responseInfo)
                                  {
                                      std::cout << _responseInfo.message << std::endl;
                                  }, claimTokenTxnHash);

    // Get Token Balance for NFT Alice
    std::cout << "..=== Get Token Balance for Alice NFT ===.." << std::endl;

    getTokenBalanceResultAlice = "";
    restClient.GetTokenBalance([&](const std::string& returnResult)
                               {
                                   getTokenBalanceResultAlice = returnResult;
                               }, *aliceAddress, *aliceAddress, collectionName, tokenName, propertyVersion);

    std::cout << "Alice's NFT Token Balance: " << getTokenBalanceResultAlice << std::endl;
    // Get Token Balance for NFT Bob
    std::cout << "..=== Get Token Balance for Bob NFT ===.." << std::endl;

    std::string getTokenBalanceResultBob = "";
    restClient.GetTokenBalance([&](const std::string& returnResult)
                               {
                                   getTokenBalanceResultBob = returnResult;
                               }, *bobAddress, *aliceAddress, collectionName, tokenName, propertyVersion);

    std::cout << "Bob's NFT Token Balance: " << getTokenBalanceResultBob << std::endl;
}

}
