#include "AptosToken.h"
#include <iostream>
#include <string>
#include <thread>
#include "REST/FaucetClient.h"
#include "REST/RestClient.h"
#include "REST/Constant.h"
#include "REST/AptosTokenClient.h"
#include "Model/AddTokenPropertyResponse.h"
#include "Model/CreateCollectionResponse.h"
#include "REST/Model/Resources/AccountResource.h"
#include "Model/CreateTokenResponse.h"
#include "Model/TransferObjectResponse.h"
#include "Model/UpdateTokenResponse.h"

namespace AptosSDKDemo {

AptosToken::AptosToken() {
    std::cout << "=== =========================== ===" << std::endl;
    std::cout << "=== AptosToken                  ===" << std::endl;
    std::cout << "=== =========================== ===" << std::endl;
}

// This code demonstrates basic Aptos blockchain token operations using the Devnet:
//
// * Sets up Faucet and REST clients for interacting with the Aptos blockchain.
// * Creates accounts for Alice and Bob, funding them through the Devnet faucet.
// * Creates a collection for Alice.
// * Mints a token within Alice's collection.
// * Adds, removes, and updates properties on the minted token.
// * Transfers the token from Alice to Bob.
void AptosToken::Start()
{
    std::cout << "=== =========================== ===" << std::endl;
    std::cout << "=== Set Up Faucet & REST Client ===" << std::endl;
    std::cout << "=== =========================== ===" << std::endl;

    std::string faucetEndpoint = "https://faucet.devnet.aptoslabs.com";

    Aptos::Rest::FaucetClient faucetClient;
    Aptos::Rest::RestClient restClient(Constants::DEVNET_BASE_URL);
    Aptos::Rest::AptosTokenClient tokenClient(restClient);
    // Create Accounts
    Aptos::Accounts::Account alice;
    Aptos::Accounts::Account bob;
    std::cout << "=== ========= ===" << std::endl;
    std::cout << "=== Addresses ===" << std::endl;
    std::cout << "=== ========= ===" << std::endl;
    std::cout << "Alice: " << alice.getAccountAddress()->ToString() << std::endl;
    std::cout << "Bob: " << bob.getAccountAddress()->ToString() << std::endl;
    // Fund Alice Account Through Devnet Faucet
    auto aliceAddress = alice.getAccountAddress();
    bool success = false;
    AptosRESTModel::ResponseInfo responseInfo;

    faucetClient.FundAccount([&](bool _success, AptosRESTModel::ResponseInfo _responseInfo) {
        success = _success;
        responseInfo = _responseInfo;
    }, aliceAddress->ToString(), 100000000, faucetEndpoint);

    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success) {
        std::cout << "Faucet funding for Alice failed: " << responseInfo.message << std::endl;
        return;
    }
    // Fund Bob Account Through Devnet Faucet
    auto bobAddress = bob.getAccountAddress();
    faucetClient.FundAccount([&](bool _success, AptosRESTModel::ResponseInfo _responseInfo) {
        success = _success;
        responseInfo = _responseInfo;
    }, bobAddress->ToString(), 100000000, faucetEndpoint);
    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success) {
        std::cout << "Faucet funding for Bob failed: " << responseInfo.message << std::endl;
        return;
    }
    // Initial Coin Balances
    AptosRESTModel::AccountResourceCoin::Coin aliceCoin;

    restClient.GetAccountBalance([&](AptosRESTModel::AccountResourceCoin::Coin _coin, AptosRESTModel::ResponseInfo _responseInfo) {
        aliceCoin = _coin;
        responseInfo = _responseInfo;
    }, *aliceAddress);

    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed) {
        std::cout << responseInfo.message << std::endl;
        return;
    }
    std::cout << "Alice's Balance After Funding: " << aliceCoin.value() << std::endl;

    AptosRESTModel::AccountResourceCoin::Coin bobCoin;
    restClient.GetAccountBalance([&](AptosRESTModel::AccountResourceCoin::Coin _coin, AptosRESTModel::ResponseInfo _responseInfo) {
        bobCoin = _coin;
        responseInfo = _responseInfo;
    }, *bobAddress);

    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed) {
        std::cerr << responseInfo.message << std::endl;
        return ;
    }
    std::cout << "Bob's Balance After Funding: " << bobCoin.value() << std::endl;
    std::string collectionName = "Alice's";
    std::string tokenName = "Alice's first token";
    // Create Collection
    std::string createCollectionTxn = "";

    tokenClient.CreateCollection([&](std::string _createCollectionTxn, AptosRESTModel::ResponseInfo _responseInfo) {
        createCollectionTxn = _createCollectionTxn;
        responseInfo = _responseInfo;
    }, alice, "Alice's simple collection", 1,  collectionName, "https://aptos.dev", true, true, true, true, true, true, true, true, true, 0, 1);

    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success) {
        std::cerr << "Cannot create collection. " << responseInfo.message << std::endl;
    }

    std::cout << "Create Collection Response: " << responseInfo.message << std::endl;
    std::cout << "Transaction Details: " << createCollectionTxn << std::endl;

    CreateCollectionResponse txnResponse = CreateCollectionResponse::from_json(nlohmann::json::parse(createCollectionTxn));

    std::string transactionHash = txnResponse.getHash();
    std::cout << "Transaction Hash: " << transactionHash << std::endl;

    bool waitForTxnSuccess = false;

    restClient.WaitForTransaction(
        [&](bool _pending, AptosRESTModel::ResponseInfo _responseInfo) {
            waitForTxnSuccess = _pending;
            responseInfo = _responseInfo;
        },
        transactionHash
        );
    if (!waitForTxnSuccess) {
        std::cerr << "Transaction was not found. Breaking out of example: Error: " << responseInfo.message << std::endl;
        return;
    }
    // Get Account Resource
    std::cout << "=== Get Account Resource for Alice ===" << std::endl;
    long responseCode = 0;
    std::string accountResourceResp = "";

    restClient.GetAccountResource([&](bool _success, long _responseCode, const std::string& _returnResult) {
        success = _success;
        responseCode = _responseCode;
        accountResourceResp = _returnResult;
    }, *alice.getAccountAddress(), "0x1::account::Account");

    if (!success) {
        if (responseCode == 404) {
            responseInfo.status = AptosRESTModel::ResponseInfo::Status::NotFound;
        } else {
            responseInfo.status = AptosRESTModel::ResponseInfo::Status::Failed;
        }
        responseInfo.message = accountResourceResp;
        std::cout << "Error: " << responseInfo.message << std::endl;
        return;
    }

    AptosRESTModel::AccountResource accountResource = AptosRESTModel::AccountResource::FromJson(nlohmann::json::parse(accountResourceResp));
    int creationNum = std::stoi(accountResource.getData().getGuidCreationNum());
    std::cout << "Creation Num: " << creationNum << std::endl;

    // Mint Token
    std::cout << "<>=== ========== ===" << std::endl;
    std::cout << "<>=== Mint Token ===" << std::endl;
    std::cout << "<>=== ========== ===" << std::endl;

    std::string mintTokenTxn = "";
    tokenClient.MintToken([&](const std::string& _mintTokenTxn, AptosRESTModel::ResponseInfo _responseInfo) {
        mintTokenTxn = _mintTokenTxn;
        responseInfo = _responseInfo;
    }, alice, collectionName, "Alice's simple token", tokenName,
                          "https://aptos.dev/img/nyan.jpeg", Aptos::Rest::PropertyMap{ {Aptos::Rest::Property::StringProp("string", "string value")} });

    if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success) {
        std::cerr << "Cannot mint token. " << responseInfo.message << std::endl;
        return;
    }

    std::cout << "Mint Token Response: " << responseInfo.message << std::endl;
    std::cout << "Transaction Details: " << mintTokenTxn << std::endl;

    CreateTokenResponse createTxnResponse = CreateTokenResponse::from_json(nlohmann::json::parse(mintTokenTxn));
    std::string createTokenTxnHash = createTxnResponse.getHash();
    std::cout << "Transaction Hash: " << createTokenTxnHash << std::endl;
    //Wait for Transaction
    waitForTxnSuccess = false;

    restClient.WaitForTransaction(
        [&](bool _pending, AptosRESTModel::ResponseInfo _responseInfo) {
            waitForTxnSuccess = _pending;
            responseInfo = _responseInfo;
        },
        createTokenTxnHash
        );
    if (!waitForTxnSuccess) {
        std::cerr << "Transaction was not found. Breaking out of example: Error: " << responseInfo.message << std::endl;
        return;
    }
    // Collection Address and Token Address
    Aptos::Accounts::AccountAddress collectionAddress = Aptos::Accounts::AccountAddress::ForNamedCollection(*alice.getAccountAddress(), collectionName);
    std::vector<Aptos::Accounts::AccountAddress> mintedTokens;
    tokenClient.TokensMintedFromTransaction([&](std::vector<Aptos::Accounts::AccountAddress> _tokenArray, AptosRESTModel::ResponseInfo _responseInfo) {
        mintedTokens = _tokenArray;
        responseInfo = _responseInfo;
    }, createTokenTxnHash);
    Aptos::Accounts::AccountAddress tokenAddress = mintedTokens[0];
    std::cout << "AccountAddress ForNamedCollection: " << collectionAddress.ToString() << std::endl;
    std::cout << "AccountAddress ForGuidObject: " << tokenAddress.ToString() << std::endl;
    // Token Client read_object Collection
    std::cout << "=== ====================== ===" << std::endl;
    std::cout << "=== Read Collection Object ===" << std::endl;
    std::cout << "=== =================== ===" << std::endl;

    Aptos::Rest::ReadObject readObjectCollection({});
    tokenClient.ReadObject([&](Aptos::Rest::ReadObject _readObjectCollection, AptosRESTModel::ResponseInfo _responseInfo) {
        readObjectCollection = _readObjectCollection;
        responseInfo = _responseInfo;
    }, collectionAddress);
    std::cout << "Alice's collection: " << readObjectCollection.ToString() << std::endl;

    // Token client read_object Token Address
    std::cout << "=== Read Token Object ===" << std::endl;

    Aptos::Rest::ReadObject readObjectToken({});

    tokenClient.ReadObject([&](Aptos::Rest::ReadObject _readObjectToken, AptosRESTModel::ResponseInfo _responseInfo) {
        readObjectToken = _readObjectToken;
        responseInfo = _responseInfo;
    }, tokenAddress);

    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::NotFound) {
        std::cerr << "ERROR: " << responseInfo.message << std::endl;
        return;
    }

    std::cout << "Alice's token: " << readObjectToken.ToString() << std::endl;
    // Add token property
    std::cout << "=== ================== ===\n"
              << "=== Add Token Property ===\n"
              << "=== ================== ===" << std::endl;
    std::string responseString;
    tokenClient.AddTokenProperty([&](std::string _responseString, AptosRESTModel::ResponseInfo _responseInfo) {
        responseString = _responseString;
        responseInfo = _responseInfo;
    }, alice, tokenAddress, Aptos::Rest::Property::BoolProp("test", false));
    AddTokenPropertyResponse addTokenPropertyResponse = AddTokenPropertyResponse::from_json(nlohmann::json::parse(responseString));
    transactionHash = addTokenPropertyResponse.getHash();
    waitForTxnSuccess = false;
    restClient.WaitForTransaction([&](bool _pending, AptosRESTModel::ResponseInfo _responseInfo) {
        waitForTxnSuccess = _pending;
        responseInfo = _responseInfo;
    }, transactionHash);

    if (!waitForTxnSuccess) {
        std::cout << "Transaction was not found. Breaking out of example: Error: " << responseInfo.message << std::endl;
        return;
    }

    tokenClient.ReadObject([&](Aptos::Rest::ReadObject _readObjectToken, AptosRESTModel::ResponseInfo _responseInfo) {
        readObjectToken = _readObjectToken;
        responseInfo = _responseInfo;
    }, tokenAddress);

    std::cout << "Alice's token: " << readObjectToken.ToString() << std::endl;
    std::cout << "=== =================== ===\n"
              << "=== Remove Property ===\n"
              << "=== =================== ===" << std::endl;

    tokenClient.RemoveTokenProperty([&](std::string _responseString, AptosRESTModel::ResponseInfo _responseInfo) {
        responseString = _responseString;
        responseInfo = _responseInfo;
    }, alice, tokenAddress, "string");

    waitForTxnSuccess = false;
    restClient.WaitForTransaction([&](bool _pending, AptosRESTModel::ResponseInfo _responseInfo) {
        waitForTxnSuccess = _pending;
        responseInfo = _responseInfo;
    }, transactionHash);

    if (!waitForTxnSuccess) {
        std::cout << "Transaction was not found. Breaking out of example: Error: " << responseInfo.message << std::endl;
        return;
    }
    tokenClient.ReadObject([&](Aptos::Rest::ReadObject _readObjectToken, AptosRESTModel::ResponseInfo _responseInfo) {
        readObjectToken = _readObjectToken;
        responseInfo = _responseInfo;
    }, tokenAddress);

    std::cout << "Alice's token: " << readObjectToken.ToString() << std::endl;
    // Update Token Property
    std::cout << "=== ================== ===\n"
              << "=== Update Token Property ===\n"
              << "=== ================== ===" << std::endl;
    tokenClient.UpdateTokenProperty([&](std::string _responseString, AptosRESTModel::ResponseInfo _responseInfo) {
        responseString = _responseString;
        responseInfo = _responseInfo;
    }, alice, tokenAddress, Aptos::Rest::Property::BoolProp("test", true));
    UpdateTokenResponse updateTokenPropertyResponse = UpdateTokenResponse::from_json(nlohmann::json::parse(responseString));
    transactionHash = updateTokenPropertyResponse.getHash();
    waitForTxnSuccess = false;
    restClient.WaitForTransaction([&](bool _pending, AptosRESTModel::ResponseInfo _responseInfo) {
        waitForTxnSuccess = _pending;
        responseInfo = _responseInfo;
    }, transactionHash);

    if (!waitForTxnSuccess) {
        std::cout << "Transaction was not found. Breaking out of example: Error: " << responseInfo.message << std::endl;
        return;
    }
    tokenClient.ReadObject([&](Aptos::Rest::ReadObject _readObjectToken, AptosRESTModel::ResponseInfo _responseInfo) {
        readObjectToken = _readObjectToken;
        responseInfo = _responseInfo;
    }, tokenAddress);

    std::cout << "Alice's token: " << readObjectToken.ToString() << std::endl;
    // Add token property -- binary data
    std::cout << "=== ==================================== ===\n"
              << "=== Add Token Property - Binary Sequence ===\n"
              << "=== ==================================== ===" << std::endl;
    tokenClient.AddTokenProperty([&](std::string _responseString, AptosRESTModel::ResponseInfo _responseInfo) {
        responseString = _responseString;
        responseInfo = _responseInfo;
    }, alice, tokenAddress, Aptos::Rest::Property::BytesProp("bytes", {0x00, 0x01}));

    addTokenPropertyResponse = AddTokenPropertyResponse::from_json(nlohmann::json::parse(responseString));
    transactionHash = addTokenPropertyResponse.getHash();

    waitForTxnSuccess = false;
    restClient.WaitForTransaction([&](bool _pending, AptosRESTModel::ResponseInfo _responseInfo) {
        waitForTxnSuccess = _pending;
        responseInfo = _responseInfo;
    }, transactionHash);

    if (!waitForTxnSuccess) {
        std::cout << "Transaction was not found. Breaking out of example: Error: " << responseInfo.message << std::endl;
        return;
    }

    tokenClient.ReadObject([&](Aptos::Rest::ReadObject _readObjectToken, AptosRESTModel::ResponseInfo _responseInfo) {
        readObjectToken = _readObjectToken;
        responseInfo = _responseInfo;
    }, tokenAddress);

    std::cout << "Alice's token: " << readObjectToken.ToString() << std::endl;
    // Transferring Tokens
    std::cout << "=== ======================================== ===\n"
              << "=== Transferring the Token from Alice to Bob ===\n"
              << "=== ======================================== ===" << std::endl;

    std::cout << "Alice: " << alice.getAccountAddress()->ToString() << std::endl;
    std::cout << "Bob: " << bob.getAccountAddress()->ToString() << std::endl;

    restClient.TransferObject([&](std::string _responseString, AptosRESTModel::ResponseInfo _responseInfo) {
        responseString = _responseString;
        responseInfo = _responseInfo;
    }, alice, tokenAddress, *bob.getAccountAddress());

    std::cout << "Response: " << responseString << std::endl;
    TransferObjectResponse transferObjectResponse = TransferObjectResponse::from_json(nlohmann::json::parse(responseString));
    transactionHash = transferObjectResponse.getHash();
    waitForTxnSuccess = false;
    restClient.WaitForTransaction([&](bool _pending, AptosRESTModel::ResponseInfo _responseInfo) {
        waitForTxnSuccess = _pending;
        responseInfo = _responseInfo;
    }, transactionHash);

    if (!waitForTxnSuccess) {
        std::cout << "Transaction was not found. Breaking out of example: Error: " << responseInfo.message << std::endl;
        return;
    }

    Aptos::Rest::ReadObject transferObjectReadObject({});
    tokenClient.ReadObject([&](Aptos::Rest::ReadObject _readObjectCollection, AptosRESTModel::ResponseInfo _responseInfo) {
        transferObjectReadObject = _readObjectCollection;
        responseInfo = _responseInfo;
    }, tokenAddress);

    std::cout << "Alice's transferred token: " << transferObjectReadObject.ToString() << std::endl;

}

}
