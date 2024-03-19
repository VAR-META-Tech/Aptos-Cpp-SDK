#include "SimulateTransferCoin.h"
#include <iostream>
#include <string>
#include "REST/RestClient.h"
#include "REST/FaucetClient.h"
#include "REST/Constant.h"
#include "REST/AptosTokenClient.h"
#include "BCS/StructTag.h"
#include "BCS/TagSequence.h"
#include "BCS/BCS.h"
#include "HDWallet/Utils/Utils.h"
#include "REST/Model/TransactionConverter.h"

using namespace Aptos::Accounts;
using namespace Aptos::BCS;
namespace AptosSDKDemo
{

    SimulateTransferCoin::SimulateTransferCoin()
    {
        std::cout << "=== =========================== ===" << std::endl;
        std::cout << "=== SimulateTransferCoin        ===" << std::endl;
        std::cout << "=== =========================== ===" << std::endl;
    }

    // This code demonstrates the use of transaction simulation on the Aptos blockchain. It highlights:
    //
    // * **Setup:** Creates accounts for Alice and Bob, funds Alice through the Devnet faucet.
    // * **Transaction Creation:** Generates a BCS transaction to transfer coins from Alice to Bob.
    // * **Simulation (Before Bob's Account Exists):** Simulates the transaction, demonstrating its expected failure due to Bob's account not yet existing.
    // * **Simulation (After Bob's Account Exists):**  Funds Bob's account and then re-simulates the transaction, showing its expected success.
    void SimulateTransferCoin::Start()
    {
        // REST & Faucet Client Setup
        std::cout << "=== =========================== ===" << std::endl;
        std::cout << "=== Set Up Faucet & REST Client ===" << std::endl;
        std::cout << "=== =========================== ===" << std::endl;

        std::string faucetEndpoint = "https://faucet.devnet.aptoslabs.com";

        Aptos::Rest::FaucetClient faucetClient;
        Aptos::Rest::RestClient restClient(Constants::DEVNET_BASE_URL);

        Aptos::Rest::AptosTokenClient tokenClient(restClient);

        // Generate Alice and Bob's Accounts and Only Fund Alice's Account
        Account alice;
        Account bob;

        std::cout << "Alice: " << alice.getAccountAddress()->ToString() << std::endl;
        std::cout << "Bob: " << bob.getAccountAddress()->ToString() << std::endl;

        // Fund Alice account
        bool success = false;
        AptosRESTModel::ResponseInfo responseInfo;

        faucetClient.FundAccount([&](bool _success, AptosRESTModel::ResponseInfo _responseInfo)
                                 {
        success = _success;
        responseInfo = _responseInfo; },
                                 alice.getAccountAddress()->ToString(), 100000000, faucetEndpoint);

        if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success)
        {
            std::cerr << "Faucet funding for Alice failed: " << responseInfo.message << std::endl;
            return;
        }

        // Create BCS Transaction
        std::vector<std::shared_ptr<ISerializable>> args = {
            bob.getAccountAddress(),
            std::make_shared<U64>(100000),
        };
        Aptos::BCS::TagSequence tags{{std::make_shared<StructTag>(StructTag::FromStr("0x1::aptos_coin::AptosCoin"))}};
        EntryFunction entryFunction = EntryFunction::Natural(ModuleId(AccountAddress::FromHex("0x1"), "coin"),
                                                             "transfer",
                                                             tags,
                                                             Sequence(args));
        std::shared_ptr<RawTransaction> rawTxn = nullptr;

        restClient.CreateBCSTransaction(
            [&rawTxn](std::shared_ptr<RawTransaction> _rawTxn)
            {
                rawTxn = _rawTxn;
            },
            alice, Aptos::BCS::TransactionPayload(std::make_shared<EntryFunction>(entryFunction)));

        // Simulate Before Creating Bob's Account
        std::cout << "=== ===================================== ===" << std::endl;
        std::cout << "=== Simulate before creatng Bob's Account ===" << std::endl;
        std::cout << "=== ===================================== ===" << std::endl;

        std::string simulateTxnResponse;
        restClient.SimulateTransaction(
            [&simulateTxnResponse, &responseInfo](std::string _respJson, AptosRESTModel::ResponseInfo _responseInfo)
            {
                simulateTxnResponse = _respJson;
                responseInfo = _responseInfo;
            },
            *rawTxn, Aptos::Utils::SecBlockToByteVector(alice.getPublicKey()->KeyBytes()));

        simulateTxnResponse = simulateTxnResponse.substr(1, simulateTxnResponse.length() - 2);

        // Deserialize the JSON string to a Transaction object using the provided TransactionConverter
        AptosRESTModel::Transaction transactionResp = AptosRESTModel::TransactionConverter::ReadJson(nlohmann::json::parse(simulateTxnResponse));

        std::cout << transactionResp.getVmStatus() << std::endl;
        std::cout << "=== This shouldn't succeed: " << !transactionResp.getVmStatus().compare("Executed successfully") << "\nVM Status:   " << transactionResp.getVmStatus() << " ===" << std::endl;
        std::cout << simulateTxnResponse << std::endl;

        std::cout << "=== ==================================== ===" << std::endl;
        std::cout << "=== Simulate after creatng Bob's Account ===" << std::endl;
        std::cout << "=== ==================================== ===" << std::endl;

        // Fund Bob's account
        success = false;
        responseInfo = AptosRESTModel::ResponseInfo();
        faucetClient.FundAccount(
            [&success, &responseInfo](bool _success, AptosRESTModel::ResponseInfo _responseInfo)
            {
                success = _success;
                responseInfo = _responseInfo;
            },
            bob.getAccountAddress()->ToString(), 0, faucetEndpoint);

        if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success)
        {
            std::cerr << "Faucet funding for Bob failed: " << responseInfo.message << std::endl;
            return;
        }

        // Simulate transaction again
        simulateTxnResponse = "";
        restClient.SimulateTransaction(
            [&simulateTxnResponse, &responseInfo](const std::string &_respJson, AptosRESTModel::ResponseInfo _responseInfo)
            {
                simulateTxnResponse = _respJson;
                responseInfo = _responseInfo;
            },
            *rawTxn, Aptos::Utils::SecBlockToByteVector(alice.getPublicKey()->KeyBytes()));

        simulateTxnResponse = simulateTxnResponse.substr(1, simulateTxnResponse.length() - 2);
        transactionResp = AptosRESTModel::TransactionConverter::ReadJson(nlohmann::json::parse(simulateTxnResponse));
        std::cout << "=== This should succeed: " << (transactionResp.getVmStatus() == "Executed successfully") << "\nVM Status:   " << transactionResp.getVmStatus() << " ===" << std::endl;
        std::cout << simulateTxnResponse << std::endl;
    }

}
