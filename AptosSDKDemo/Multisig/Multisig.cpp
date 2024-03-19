#include "Multisig.h"
#include <iostream>
#include <string>
#include <sstream>
#include "REST/RestClient.h"
#include "REST/FaucetClient.h"
#include "REST/Constant.h"
#include "REST/AptosTokenClient.h"
#include "BCS/StructTag.h"
#include "BCS/BCS.h"
#include "REST/Constant.h"
#include "HDWallet/Utils/Utils.h"
#include "Model/MultisigTransferBcsTransactionResponse.h"
#include "Accounts/RotationProofChallenge.h"
#include "Model/RotateKeyBcsTransactionResponse.h"

using namespace Aptos::Accounts;
using namespace Aptos::BCS;
using namespace Aptos::Accounts::Ed25519;
namespace AptosSDKDemo
{

    Multisig::Multisig()
    {
        std::cout << "=== =========================== ===" << std::endl;
        std::cout << "=== Multisig Example            ===" << std::endl;
        std::cout << "=== =========================== ===" << std::endl;
    }

    // This code demonstrates multisig (multi-signature) operations on the Aptos blockchain using the Devnet:
    //
    // * **Account Setup:** Generates accounts for Alice, Bob, Chad, a multisig account, and a vanity address ("Deedee").
    // * **Multisig Creation:** Sets up a 2-out-of-3 multisig account, meaning 2 of the 3 associated signatures are required for transactions.
    // * **Funding:** Funds accounts through the Devnet faucet.
    // * **Transfer Initiation:** Initiates a coin transfer from the multisig account to Chad's account.
    // * **Signing:** Collects individual signatures from Alice and Bob for the transfer.
    // * **Submission:** Submits the signed multisig transaction to the blockchain.
    // * **Vanity Funding:** Funds the "Deedee" vanity address.
    // * **Key Rotation:** Demonstrates rotating the authentication key of the "Deedee" account to the multisig public key.
    void Multisig::Start()
    {
        std::cout << "=== =========================== ===" << std::endl;
        std::cout << "=== Set Up Faucet & REST Client ===" << std::endl;
        std::cout << "=== =========================== ===" << std::endl;

        std::string faucetEndpoint = "https://faucet.devnet.aptoslabs.com";
        Aptos::Rest::FaucetClient faucetClient;
        Aptos::Rest::RestClient restClient(Constants::DEVNET_BASE_URL);
        Aptos::Rest::AptosTokenClient aptosTokenClient(restClient);

        // Section 1. Generate accounts
        std::cout << "=== ================= ===" << std::endl;
        std::cout << "=== Generate Accounts ===" << std::endl;
        std::cout << "=== ================= ===" << std::endl;

        Account alice;
        Account bob;
        Account chad;
        std::cout << "=== Account addresses ===" << std::endl;
        std::cout << "Alice: " << alice.getAccountAddress()->ToString() << std::endl;
        std::cout << "Bob: " << bob.getAccountAddress()->ToString() << std::endl;
        std::cout << "Chad: " << chad.getAccountAddress()->ToString() << std::endl;
        std::cout << "=== Authentication keys ===" << std::endl;
        std::cout << "Alice: " << alice.AuthKey() << std::endl;
        std::cout << "Bob: " << bob.AuthKey() << std::endl;
        std::cout << "Chad: " << chad.AuthKey() << std::endl;
        std::cout << "=== Public keys ===" << std::endl;
        std::cout << "Alice: " << alice.getPublicKey()->ToString() << std::endl;
        std::cout << "Bob: " << bob.getPublicKey()->ToString() << std::endl;
        std::cout << "Chad: " << chad.getPublicKey()->ToString() << std::endl;

        // Section 2. Create MultiPublicKey and MultiSig
        std::cout << "=== ================= ===" << std::endl;
        std::cout << "=== Create MultiSig ===" << std::endl;
        std::cout << "=== ================= ===" << std::endl;
        CryptoPP::byte threshold = 2;
        auto alicePublicKey = alice.getPublicKey();
        auto bobPublicKey = bob.getPublicKey();
        auto chadPublicKey = chad.getPublicKey();
        MultiPublicKey multisigPublicKey({*alicePublicKey, *bobPublicKey, *chadPublicKey}, threshold);
        AccountAddress multisigAddress = AccountAddress::FromMultiEd25519(multisigPublicKey);
        std::cout << "=== ======================= ===" << std::endl;
        std::cout << "=== 2-of-3 Multisig account ===" << std::endl;
        std::cout << "=== ======================= ===" << std::endl;
        std::cout << "Account public key: " << multisigPublicKey.ToString() << std::endl;
        std::cout << "Account address: " << multisigAddress.ToString() << std::endl;

        // Funding Accounts

        int aliceStart = 10000000;
        int bobStart = 20000000;
        int chadStart = 30000000;
        int multisigStart = 40000000;

        // Fund Alice account
        AptosRESTModel::ResponseInfo responseInfoAlice;
        AptosRESTModel::AccountResourceCoin::Coin coinAlice;
        faucetClient.FundAccount([&](bool success, AptosRESTModel::ResponseInfo responseInfo)
                                 { responseInfoAlice = responseInfo; },
                                 alice.getAccountAddress()->ToString(), aliceStart, faucetEndpoint);

        if (responseInfoAlice.status != AptosRESTModel::ResponseInfo::Status::Success)
        {
            std::cout << "Faucet funding for Alice failed: " << responseInfoAlice.message << std::endl;
            return;
        }

        restClient.GetAccountBalance([&](AptosRESTModel::AccountResourceCoin::Coin coin, AptosRESTModel::ResponseInfo responseInfo)
                                     {
        coinAlice = coin;
        responseInfoAlice = responseInfo; },
                                     *alice.getAccountAddress());

        if (responseInfoAlice.status == AptosRESTModel::ResponseInfo::Status::Failed)
        {
            std::cout << responseInfoAlice.message << std::endl;
            return;
        }

        std::cout << "Alice's balance: " + coinAlice.value() << std::endl;
        AptosRESTModel::ResponseInfo responseInfoBob;
        AptosRESTModel::AccountResourceCoin::Coin coinBob;
        faucetClient.FundAccount([&](bool success, AptosRESTModel::ResponseInfo responseInfo)
                                 { responseInfoBob = responseInfo; },
                                 bob.getAccountAddress()->ToString(), bobStart, faucetEndpoint);

        if (responseInfoBob.status != AptosRESTModel::ResponseInfo::Status::Success)
        {
            std::cout << "Faucet funding for Bob failed: " << responseInfoBob.message << std::endl;
            return;
        }

        restClient.GetAccountBalance([&](AptosRESTModel::AccountResourceCoin::Coin coin, AptosRESTModel::ResponseInfo responseInfo)
                                     {
        coinBob = coin;
        responseInfoBob = responseInfo; },
                                     *bob.getAccountAddress());

        if (responseInfoBob.status == AptosRESTModel::ResponseInfo::Status::Failed)
        {
            std::cout << responseInfoBob.message << std::endl;
            return;
        }

        std::cout << "Bob's balance: " << coinBob.value() << std::endl;

        AptosRESTModel::ResponseInfo responseInfoChad;
        AptosRESTModel::AccountResourceCoin::Coin coinChad;
        faucetClient.FundAccount([&](bool success, AptosRESTModel::ResponseInfo responseInfo)
                                 { responseInfoChad = responseInfo; },
                                 chad.getAccountAddress()->ToString(), chadStart, faucetEndpoint);

        if (responseInfoChad.status != AptosRESTModel::ResponseInfo::Status::Success)
        {
            std::cout << "Faucet funding for Chad failed: " << responseInfoChad.message << std::endl;
            return;
        }

        restClient.GetAccountBalance([&](AptosRESTModel::AccountResourceCoin::Coin coin, AptosRESTModel::ResponseInfo responseInfo)
                                     {
        coinChad = coin;
        responseInfoChad = responseInfo; },
                                     *chad.getAccountAddress());

        if (responseInfoChad.status == AptosRESTModel::ResponseInfo::Status::Failed)
        {
            std::cout << responseInfoChad.message << std::endl;
            return;
        }

        std::cout << "Chad's balance: " << coinChad.value() << std::endl;

        // Fund Multisig account
        AptosRESTModel::ResponseInfo responseInfoMultisig;
        AptosRESTModel::AccountResourceCoin::Coin coinMultisig;
        faucetClient.FundAccount([&](bool success, AptosRESTModel::ResponseInfo responseInfo)
                                 { responseInfoMultisig = responseInfo; },
                                 multisigAddress.ToString(), multisigStart, faucetEndpoint);

        if (responseInfoMultisig.status != AptosRESTModel::ResponseInfo::Status::Success)
        {
            std::cout << "Faucet funding for Multisig failed: " << responseInfoMultisig.message << std::endl;
            return;
        }

        restClient.GetAccountBalance([&](AptosRESTModel::AccountResourceCoin::Coin coin, AptosRESTModel::ResponseInfo responseInfo)
                                     {
        coinMultisig = coin;
        responseInfoMultisig = responseInfo; },
                                     multisigAddress);

        if (responseInfoMultisig.status == AptosRESTModel::ResponseInfo::Status::Failed)
        {
            std::cout << responseInfoMultisig.message << std::endl;
            return;
        }

        std::cout << "Multisig's balance: " << coinMultisig.value() << std::endl;

        // Section 4. Initiate Transfer

        std::cout << "=== ================= ===" << std::endl;
        std::cout << "=== Initiate Transfer ===" << std::endl;
        std::cout << "=== ================= ===" << std::endl;

        // Transaction Arguments
        auto chadAccountAddress = chad.getAccountAddress();
        std::vector<std::shared_ptr<ISerializable>> transactionArguments = {
            chadAccountAddress,
            std::make_shared<U64>(100),
        };
        Aptos::BCS::TagSequence tags{{std::make_shared<StructTag>(StructTag::FromStr("0x1::aptos_coin::AptosCoin"))}};
        EntryFunction entryFunction = EntryFunction::Natural(ModuleId(AccountAddress::FromHex("0x1"), "coin"),
                                                             "transfer",
                                                             tags,
                                                             Sequence(transactionArguments));

        std::time_t currentTimestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::time_t expirationTimestamp = currentTimestamp + Aptos::Rest::ClientConfig::EXPIRATION_TTL;

        RawTransaction rawTransaction(
            multisigAddress,
            0,
            Aptos::BCS::TransactionPayload(std::make_shared<EntryFunction>(entryFunction)),
            Constants::MAX_GAS_AMOUNT,
            Constants::GAS_UNIT_PRICE,
            expirationTimestamp,
            static_cast<int>(restClient.ChainId()));

        std::cout << "RAW TXN: " << rawTransaction.ToString() << std::endl;

        Ed25519Signature aliceSignature = alice.Sign(Aptos::Utils::ByteVectorToSecBlock(rawTransaction.Keyed()));
        Ed25519Signature bobSignature = bob.Sign(Aptos::Utils::ByteVectorToSecBlock(rawTransaction.Keyed()));

        if (!rawTransaction.Verify(*alice.getPublicKey(), aliceSignature))
        {
            std::cout << "Alice signature cannot be verified" << std::endl;
            return;
        }
        if (!rawTransaction.Verify(*bob.getPublicKey(), bobSignature))
        {
            std::cout << "Bob's signature cannot be verified" << std::endl;
            return;
        }

        std::cout << "=== ===================== ===" << std::endl;
        std::cout << "=== Individual signatures ===" << std::endl;
        std::cout << "=== ===================== ===" << std::endl;

        std::cout << "Alice signature: " << aliceSignature.ToString() << std::endl;
        std::cout << "Bob signature: " << bobSignature.ToString() << std::endl;

        // Section 5: Submit transfer transaction
        std::pair<PublicKey, Ed25519Signature> aliceTuple(*alicePublicKey, aliceSignature);
        std::pair<PublicKey, Ed25519Signature> bobTuple(*bobPublicKey, bobSignature);

        std::vector<std::pair<PublicKey, Ed25519Signature>> SignatureMap = {
            aliceTuple,
            bobTuple};

        MultiSignature multisigSignature(multisigPublicKey, SignatureMap);

        MultiEd25519Authenticator multiEd25519Authenticator(multisigPublicKey, multisigSignature);

        Authenticator authenticator(std::make_shared<MultiEd25519Authenticator>(multiEd25519Authenticator));

        SignedTransaction signedTransaction(rawTransaction, authenticator);

        std::cout << "SIGNED TXN: " << signedTransaction.ToString() << std::endl;

        std::cout << "=== =============================== ===" << std::endl;
        std::cout << "=== Submitting transfer transaction ===" << std::endl;
        std::cout << "=== =============================== ===" << std::endl;
        std::string submitBcsTxnJsonResponse;
        AptosRESTModel::ResponseInfo responseInfo;
        restClient.SubmitBCSTransaction([&](const std::string &responseJson, const AptosRESTModel::ResponseInfo &_responseInfo)
                                        {
                                        submitBcsTxnJsonResponse = responseJson;
                                        responseInfo = _responseInfo; },
                                        signedTransaction);

        std::cout << "SUBMIT BCS TXN: " << submitBcsTxnJsonResponse << std::endl;

        MultisigTransferBcsTransactionResponse transferTxnResponse;
        transferTxnResponse = MultisigTransferBcsTransactionResponse::from_json(nlohmann::json::parse(submitBcsTxnJsonResponse));
        std::string transactionHash = transferTxnResponse.getHash();
        bool waitForTxnSuccess = false;
        restClient.WaitForTransaction([&](bool pending, const AptosRESTModel::ResponseInfo &_responseInfo)
                                      {
        waitForTxnSuccess = pending;
        responseInfo = _responseInfo; },
                                      transactionHash);

        if (!waitForTxnSuccess)
        {
            std::cout << "Transaction was not found. Breaking out of example: Error: " << responseInfo.message << std::endl;
            return;
        }
        std::cout << "Transaction hash: " << transactionHash << std::endl;

        // Section 6: New Account Balances
        std::cout << "=== ==================== ===" << std::endl;
        std::cout << "=== New Account Balances ===" << std::endl;
        std::cout << "=== ==================== ===" << std::endl;

        // Get account balance for Alice
        AptosRESTModel::AccountResourceCoin::Coin coin;

        restClient.GetAccountBalance([&coin, &responseInfo](const AptosRESTModel::AccountResourceCoin::Coin &_coin, const AptosRESTModel::ResponseInfo &_responseInfo)
                                     {
                                               coin = _coin;
                                               responseInfo = _responseInfo; },
                                     *alice.getAccountAddress());

        if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed)
        {
            std::cerr << responseInfo.message << std::endl;
            return;
        }

        std::cout << "Alice's balance: " << coin.value() << std::endl;

        restClient.GetAccountBalance([&coin, &responseInfo](const AptosRESTModel::AccountResourceCoin::Coin &_coin, const AptosRESTModel::ResponseInfo &_responseInfo)
                                     {
                                     coin = _coin;
                                     responseInfo = _responseInfo; },
                                     *bob.getAccountAddress());

        if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed)
        {
            std::cerr << responseInfo.message << std::endl;
            return;
        }

        std::cout << "Bob's balance: " << coin.value() << std::endl;

        restClient.GetAccountBalance([&coin, &responseInfo](const AptosRESTModel::AccountResourceCoin::Coin &_coin, const AptosRESTModel::ResponseInfo &_responseInfo)
                                     {
                                     coin = _coin;
                                     responseInfo = _responseInfo; },
                                     *chad.getAccountAddress());

        if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed)
        {
            std::cerr << responseInfo.message << std::endl;
            return;
        }

        std::cout << "Chad's balance: " << coin.value() << std::endl;

        restClient.GetAccountBalance([&coin, &responseInfo](const AptosRESTModel::AccountResourceCoin::Coin &_coin, const AptosRESTModel::ResponseInfo &_responseInfo)
                                     {
                                     coin = _coin;
                                     responseInfo = _responseInfo; },
                                     multisigAddress);
        if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed)
        {
            std::cerr << responseInfo.message << std::endl;
            return;
        }
        std::cout << "Multisig balance: " << coin.value() << std::endl;

        // Section 7: Funding Vanity Address
        std::cout << "=== ====================== ===" << std::endl;
        std::cout << "=== Funding vanity address ===" << std::endl;
        std::cout << "=== ====================== ===" << std::endl;

        std::shared_ptr<Account> deedee = std::make_shared<Account>();

        while (deedee->getAccountAddress()->ToString().substr(2, 2) == "dd")
        {
            deedee = std::make_shared<Account>();
        }

        std::cout << "Deedee's address: " << deedee->getAccountAddress()->ToString() << std::endl;
        std::cout << "Deedee's public key: " << deedee->getPublicKey()->ToString() << std::endl;

        int deedeeStart = 50000000;
        bool success = false;
        faucetClient.FundAccount([&success, &responseInfo](bool _success, const AptosRESTModel::ResponseInfo &_responseInfo)
                                 {
            success = _success;
            responseInfo = _responseInfo; },
                                 deedee->getAccountAddress()->ToString(), deedeeStart, faucetEndpoint);

        if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success)
        {
            std::cerr << "Faucet funding for Deedee failed: " << responseInfo.message << std::endl;
            return;
        }

        restClient.GetAccountBalance([&coin, &responseInfo](const AptosRESTModel::AccountResourceCoin::Coin &_coin, const AptosRESTModel::ResponseInfo &_responseInfo)
                                     {
                                     coin = _coin;
                                     responseInfo = _responseInfo; },
                                     *deedee->getAccountAddress());

        if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Failed)
        {
            std::cerr << responseInfo.message << std::endl;
            return;
        }
        std::cout << "Deedee's balance: " << coin.value() << std::endl;
        // Section 8: Signing Rotation Proof Challenge
        std::cout << "=== =============================== ===" << std::endl;
        std::cout << "=== Signing rotation proof challenge ===" << std::endl;
        std::cout << "=== =============================== ===" << std::endl;

        RotationProofChallenge rotationProofChallenge(
            0,
            *deedee->getAccountAddress(),
            *deedee->getAccountAddress(),
            multisigPublicKey.ToBytes());

        Serialization serializer;
        rotationProofChallenge.Serialize(serializer);
        auto rotationProofChallangeBcs = Aptos::Utils::ByteVectorToSecBlock(serializer.GetBytes());

        auto capRotateKey = deedee->Sign(rotationProofChallangeBcs).Data();

        MultiSignature multiSignature(
            multisigPublicKey,
            {std::make_pair(*bob.getPublicKey(), bob.Sign(rotationProofChallangeBcs)),
             std::make_pair(*chad.getPublicKey(), chad.Sign(rotationProofChallangeBcs))});

        std::vector<uint8_t> capUpdateTable = multiSignature.ToBytes();

        std::ostringstream capRotateKeyHexStream;
        capRotateKeyHexStream << "0x";
        for (const auto &byte : capRotateKey)
        {
            capRotateKeyHexStream << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
        }
        std::string capRotateKeyHex = capRotateKeyHexStream.str();

        std::ostringstream capUpdateTableHexStream;
        capUpdateTableHexStream << "0x";
        for (const auto &byte : capUpdateTable)
        {
            capUpdateTableHexStream << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
        }
        std::string capUpdateTableHex = capUpdateTableHexStream.str();

        std::cout << capRotateKeyHex << std::endl;
        std::cout << capUpdateTableHex << std::endl;

        // Section 9: Submitting Authentication Key Rotation Transaction
        std::cout << "=== ================================================== ===" << std::endl;
        std::cout << "=== Submitting authentication key rotation transaction ===" << std::endl;
        std::cout << "=== ================================================== ===" << std::endl;

        int fromScheme = Authenticator::ED25519;
        auto fromPublicKeyBytes = deedee->getPublicKey()->KeyBytes();
        int toScheme = Authenticator::MULTI_ED25519;
        std::vector<uint8_t> toPublicKeyBytes = multisigPublicKey.ToBytes();

        // Transaction Arguments
        std::vector<std::shared_ptr<ISerializable>> transactionArgumentsKeyRotation = {
            std::make_shared<U8>(static_cast<uint8_t>(fromScheme)),
            std::make_shared<Bytes>(Aptos::Utils::SecBlockToByteVector(fromPublicKeyBytes)),
            std::make_shared<U8>(static_cast<uint8_t>(toScheme)),
            std::make_shared<Bytes>(toPublicKeyBytes),
            std::make_shared<Bytes>(Aptos::Utils::SecBlockToByteVector(capRotateKey)),
            std::make_shared<Bytes>(capUpdateTable)};

        ModuleId moduleId(AccountAddress::FromHex("0x1"), "account");
        EntryFunction entryFunctionKeyRotation = EntryFunction::Natural(
            moduleId,
            "rotate_authentication_key",
            TagSequence(),
            Sequence(transactionArgumentsKeyRotation));
        std::shared_ptr<SignedTransaction> signedTransactionKeyRotation;
        restClient.CreateBCSSignedTransaction([&](std::shared_ptr<SignedTransaction> _signedTransaction)
                                              { signedTransactionKeyRotation = _signedTransaction; },
                                              *deedee, Aptos::BCS::TransactionPayload(std::make_shared<EntryFunction>(entryFunctionKeyRotation)));

        std::shared_ptr<AptosRESTModel::AccountData> accountData;
        restClient.GetAccount([&](std::shared_ptr<AptosRESTModel::AccountData> _accountData, AptosRESTModel::ResponseInfo _responseInfo)
                              {
        accountData = _accountData;
        responseInfo = _responseInfo; },
                              *deedee->getAccountAddress());

        if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success)
        {
            std::cerr << "Account not found." << std::endl;
            return;
        }

        std::string authKey = accountData->getAuthenticationKey();
        std::cout << "Auth key pre-rotation: " << authKey << std::endl;

        restClient.SubmitBCSTransaction([&](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
                                        {
        submitBcsTxnJsonResponse = _responseJson;
        responseInfo = _responseInfo; },
                                        *signedTransactionKeyRotation);

        std::cout << "Submit BCS Transaction:\n"
                  << submitBcsTxnJsonResponse << std::endl;
        auto bcsTxnResponse = RotateKeyBcsTransactionResponse::from_json(nlohmann::json::parse(submitBcsTxnJsonResponse));
        transactionHash = bcsTxnResponse.getHash();
        waitForTxnSuccess = false;

        restClient.WaitForTransaction([&](bool _pending, AptosRESTModel::ResponseInfo _responseInfo)
                                      {
        waitForTxnSuccess = _pending;
        responseInfo = _responseInfo; },
                                      transactionHash);

        if (!waitForTxnSuccess)
        {
            std::cout << "Transaction was not found. Breaking out of example: Error: " << responseInfo.message << std::endl;
            return;
        }

        std::cout << "Transaction hash: " << transactionHash << std::endl;

        restClient.GetAccount([&](std::shared_ptr<AptosRESTModel::AccountData> _accountData, AptosRESTModel::ResponseInfo _responseInfo)
                              {
        accountData = _accountData;
        responseInfo = _responseInfo; },
                              *deedee->getAccountAddress());

        if (responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success)
        {
            std::cerr << "Account not found." << std::endl;
            return;
        }

        authKey = accountData->getAuthenticationKey();
        std::cout << "New auth key: " << authKey << std::endl;
        std::cout << "1st multisig address: " << multisigAddress.ToString() << std::endl;
    }

}
