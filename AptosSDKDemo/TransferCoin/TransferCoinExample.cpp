#include "TransferCoinExample.h"
#include <ostream>
#include <string>
#include "REST/RestClient.h"
#include "REST/FaucetClient.h"
#include "REST/Constant.h"
#include "HDWallet/Wallet.h"
#include "Model/TransferCoinBCSResponse.h"

using namespace Aptos::Accounts;
namespace AptosSDKDemo {

TransferCoinExample::TransferCoinExample() {
    std::cout << "=== =========================== ===" << std::endl;
    std::cout << "=== TransferCoinExample ===" << std::endl;
    std::cout << "=== =========================== ===" << std::endl;
}

// This code demonstrates basic coin transfers on the Aptos blockchain using the Devnet:
//
// * **Setup:**  Creates mnemonic-based wallets for Alice & Bob, funds them using the Devnet faucet.
// * **Balances:** Demonstrates how to fetch account balances.
// * **Transfer (REST):**  Executes a coin transfer from Alice to Bob using the REST API.
// * **Transfer (BCS):**  Performs another transfer from Alice to Bob using a BCS transaction. 
void TransferCoinExample::Start()
{
    std::string mnemo = "stadium valid laundry unknown tuition train december camera fiber vault sniff ripple";
    // Set up Faucet and REST Client
    std::cout << "=== =========================== ===" << std::endl;
    std::cout << "=== Set Up Faucet & REST Client ===" << std::endl;
    std::cout << "=== =========================== ===" << std::endl;

    std::string faucetEndpoint = "https://faucet.devnet.aptoslabs.com";
    Aptos::Rest::FaucetClient faucetClient;
    Aptos::Rest::RestClient restClient(Constants::DEVNET_BASE_URL);
    Aptos::HDWallet::Wallet wallet(mnemo);

    // Alice Account
    std::cout << "=== ========= ===" << std::endl;
    std::cout << "=== Addresses ===" << std::endl;
    std::cout << "=== ========= ===" << std::endl;
    Account alice = wallet.GetDerivedAccount(0);
    std::string authKey = alice.AuthKey();
    std::cout << "Alice Auth Key: " << authKey << std::endl;
    auto aliceAddress = alice.getAccountAddress();
    std::cout << "Alice's Account Address: " << aliceAddress->ToString() << std::endl;
    auto privateKey = alice.getPrivateKey();
    std::cout << "Alice Private Key: " << privateKey->ToString() << std::endl;

    // Bob Account
    Account bob = wallet.GetDerivedAccount(1);
    auto bobAddress = bob.getAccountAddress();
    std::cout << "Bob's Account Address: " << bobAddress->ToString() << std::endl;
    std::cout << "Wallet: Account 0: Alice: " << aliceAddress->ToString() << std::endl;
    std::cout << "Wallet: Account 1: Bob: " << bobAddress->ToString() << std::endl;

    std::cout << "=== ================ ===" << std::endl;
    std::cout << "=== Initial Balances ===" << std::endl;
    std::cout << "=== ================ ===" << std::endl;

    // Get Alice Account Balance
    AptosRESTModel::ResponseInfo responseInfo;
    AptosRESTModel::AccountResourceCoin::Coin coin;

    restClient.GetAccountBalance([&](AptosRESTModel::AccountResourceCoin::Coin _coin, AptosRESTModel::ResponseInfo _responseInfo) {
        coin = _coin;
        responseInfo = _responseInfo;
    }, *aliceAddress);

    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed) {
        std::cout << responseInfo.message << std::endl;
        return;
    }
    std::cout << "Alice's Balance Before Funding: " << coin.value() << std::endl;

    // Fund Alice Account Through Devnet Faucet
    bool success = false;
    faucetClient.FundAccount([&](bool _success, AptosRESTModel::ResponseInfo _responseInfo) {
        success = _success;
        responseInfo = _responseInfo;
    }, aliceAddress->ToString(), 100000000, faucetEndpoint);
    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success) {
        std::cerr << "Faucet funding for Alice failed: " << responseInfo.message << std::endl;
        return;
    }

    // Get Alice Account Balance After Funding
    restClient.GetAccountBalance([&](AptosRESTModel::AccountResourceCoin::Coin _coin, AptosRESTModel::ResponseInfo _responseInfo) {
        coin = _coin;
        responseInfo = _responseInfo;
    }, *aliceAddress);
    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed) {
        std::cerr << "Error: " << responseInfo.message << std::endl;
        return;
    }
    std::cout << "Alice's Balance After Funding: " << coin.value() << std::endl;

    // Fund Bob Account Through Devnet Faucet
    faucetClient.FundAccount([&](bool _success, AptosRESTModel::ResponseInfo _responseInfo) {
        success = _success;
        responseInfo = _responseInfo;
    }, bobAddress->ToString(), 100000000, faucetEndpoint);
    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success) {
        std::cerr << "Faucet funding for Bob failed: " << responseInfo.message << std::endl;
        return;
    }

    // Get Bob Account Balance After Funding
    restClient.GetAccountBalance([&](AptosRESTModel::AccountResourceCoin::Coin _coin, AptosRESTModel::ResponseInfo _responseInfo) {
        coin = _coin;
        responseInfo = _responseInfo;
    }, *bobAddress);
    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed) {
        std::cerr << "Error: " << responseInfo.message << std::endl;
        return;
    }
    std::cout << "Bob's Balance After Funding: " << coin.value() << std::endl;

    // Have Alice give Bob 1_000 coins - Submit Transfer Transaction
    std::shared_ptr<AptosRESTModel::Transaction> transferTxn;
    restClient.Transfer([&](std::shared_ptr<AptosRESTModel::Transaction> _transaction, AptosRESTModel::ResponseInfo _responseInfo) {
        transferTxn = _transaction;
        responseInfo = _responseInfo;
    }, alice, bobAddress->ToString(), 1000);
    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success) {
        std::cerr << "Warning: Transfer failed: " << responseInfo.message << std::endl;
        return;
    }
    std::cout << "Transfer Response: " << responseInfo.message << std::endl;
    std::cout << "Transfer Response Hash: " << transferTxn->getHash() << std::endl;

    // Wait For Transaction
    bool waitForTxnSuccess = false;
    restClient.WaitForTransaction([&](bool _pending, AptosRESTModel::ResponseInfo _responseInfo) {
        waitForTxnSuccess = _pending;
        responseInfo = _responseInfo;
    }, transferTxn->getHash());

    if (!waitForTxnSuccess) {
        std::cerr << "Warning: Transaction was not found. Breaking out of example" << std::endl;
        return ;
    }

    std::cout << "=== ===================== ===" << std::endl;
    std::cout << "=== Intermediate Balances ===" << std::endl;
    std::cout << "=== ===================== ===" << std::endl;

    //Get Alice Account Balance After Transfer
    restClient.GetAccountBalance([&](AptosRESTModel::AccountResourceCoin::Coin _coin, AptosRESTModel::ResponseInfo _responseInfo) {
        coin = _coin;
        responseInfo = _responseInfo;
    }, *aliceAddress);

    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed) {
        std::cerr << "Error: " << responseInfo.message << std::endl;
        return;
    }

    std::cout << "Alice Balance After Transfer: " << coin.value() << std::endl;

    // Get Bob Account Balance After Transfer
    restClient.GetAccountBalance([&](AptosRESTModel::AccountResourceCoin::Coin _coin, AptosRESTModel::ResponseInfo _responseInfo) {
        coin = _coin;
        responseInfo = _responseInfo;
    }, *bobAddress);

    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed) {
        std::cerr << "Error: " << responseInfo.message << std::endl;
        return;
    }
    std::cout << "Bob Balance After Transfer: " << coin.value() << std::endl;

    // Have Alice give Bob another 1_000 coins using BCS
    std::string responseStr;
    TransferCoinBCSResponse response;

    restClient.BCSTransfer([&](std::string _responseStr, AptosRESTModel::ResponseInfo _responseInfo) {
        responseStr = _responseStr;
        responseInfo = _responseInfo;
    }, alice, *bobAddress, 1000);

    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success) {
        std::cerr << "Error: " << responseInfo.message << std::endl;
        return;
    }
    std::cout << "BCS TRANSFER: " << responseStr << std::endl;
    nlohmann::json responseObj = nlohmann::json::parse(responseStr);
    response = TransferCoinBCSResponse::from_json(responseObj);
    restClient.WaitForTransaction([&](bool _pending, AptosRESTModel::ResponseInfo _responseInfo) {
        waitForTxnSuccess = _pending;
        responseInfo = _responseInfo;
    }, response.getHash());

    if (!waitForTxnSuccess) {
        std::cerr << "Warning: Transaction was not found. Breaking out of example" << std::endl;
        return;
    }
    std::cout << "=== ============== ===" << std::endl;
    std::cout << "=== Final Balances ===" << std::endl;
    std::cout << "=== ============== ===" << std::endl;

    // Final Balances
    restClient.GetAccountBalance([&](AptosRESTModel::AccountResourceCoin::Coin _coin, AptosRESTModel::ResponseInfo _responseInfo) {
        coin = _coin;
        responseInfo = _responseInfo;
    }, *aliceAddress);

    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed) {
        std::cerr << "Error: " << responseInfo.message << std::endl;
        return;
    }

    std::cout << "Alice Balance After Transfer: " << coin.value() << std::endl;

    // Get Bob's account balance after transfer
    restClient.GetAccountBalance([&](AptosRESTModel::AccountResourceCoin::Coin _coin, AptosRESTModel::ResponseInfo _responseInfo) {
        coin = _coin;
        responseInfo = _responseInfo;
    }, *bobAddress);

    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed) {
        std::cerr << "Error: " << responseInfo.message << std::endl;
        return;
    }
    std::cout << "Bob Balance After Transfer: " << coin.value() << std::endl;
}

}

