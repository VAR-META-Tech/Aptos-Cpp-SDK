#include "RestClient.h"
#include "Model/TableItemRequest.h"
#include "Model/TableItemRequestNFT.h"
#include "Model/TableItemRequestTokenData.h"
#include "../Transactions/Authenticator.h"
#include "../Transactions/SignedTransaction.h"
#include "../HDWallet/Utils/Utils.h"
#include "Constant.h"
#include <thread>
#include "../BCS/BCSTypes.h"
#include "Model/AccountResourceTokenStore.h"
#include "RequestClient.h"
#include "Model/TransactionRequestConverter.h"
#include "Model/TransactionConverter.h"

using namespace Aptos::Accounts;
using namespace Aptos::BCS;
namespace Aptos::Rest
{
void RestClient::SetEndpoint(const std::string &url)
{
    endpoint = url;
    SetUp();
}

RestClient::RestClient(const std::string &url) {
    SetEndpoint(url);
}

void RestClient::SetUp()
{
    using namespace AptosRESTModel;
    std::shared_ptr<AptosRESTModel::LedgerInfo> ledgerInfo;
    ResponseInfo responseInfo;
    GetInfo([&](std::shared_ptr<AptosRESTModel::LedgerInfo> _ledgerInfo, ResponseInfo _responseInfo)
            {
                ledgerInfo = _ledgerInfo;
                responseInfo = _responseInfo;
                std::cerr << responseInfo.message << std::endl;
            });
    if (ledgerInfo) {
        m_ChainId = ledgerInfo->getChainId();
    } else {
        m_ChainId = 0;
    }
}

void RestClient::GetAccount(std::function<void(std::shared_ptr<AptosRESTModel::AccountData>, AptosRESTModel::ResponseInfo)> callback, const AccountAddress &accountAddress)
{
    using namespace AptosRESTModel;

    std::string uri = endpoint + "/v1/accounts/" + accountAddress.ToString();
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::GET);
    request.headers().set_content_type(U("application/json"));
    client.request(request).then([callback](web::http::http_response response)
    {
        ResponseInfo responseInfo;
        if (response.status_code() == 200)
        {
            responseInfo.status = ResponseInfo::Status::Success;
            responseInfo.message = response.extract_utf8string().get();
            AccountData accountData = AccountData::FromJson(nlohmann::json::parse(responseInfo.message));

            callback(std::make_shared<AccountData>(accountData), responseInfo);
        }
        else if (response.status_code() == 404)
        {
            responseInfo.status = ResponseInfo::Status::NotFound;
            responseInfo.message = "Account not found";
            callback(nullptr, responseInfo);
        }
        else
        {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = response.extract_utf8string().get();
            callback(nullptr, responseInfo);
        }
    }).wait();
}

void RestClient::GetAccountSequenceNumber(std::function<void(std::string, AptosRESTModel::ResponseInfo)> callback, const AccountAddress &accountAddress)
{
    using namespace AptosRESTModel;
    std::shared_ptr<AccountData> accountData;
    ResponseInfo responseInfo;

    GetAccount([&accountData, &responseInfo](std::shared_ptr<AccountData> _accountData, ResponseInfo _responseInfo)
               {
                   accountData = _accountData;
                   responseInfo = _responseInfo; },
               accountAddress);

    if (responseInfo.status != ResponseInfo::Status::Success)
    {
        callback("", responseInfo);
        return;
    }

    std::string sequenceNumber = accountData->getSequenceNumber();
    callback(sequenceNumber, responseInfo);
}

void RestClient::GetAccountBalance(std::function<void(AptosRESTModel::AccountResourceCoin::Coin, AptosRESTModel::ResponseInfo)> callback,
                                   const AccountAddress &accountAddress)
{
    using namespace AptosRESTModel;
    std::string uri = endpoint + "/v1/accounts/" + accountAddress.ToString() + "/resource/" + Constants::APTOS_COIN_TYPE;
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::GET);
    request.headers().set_content_type(U("application/json"));
    client.request(request).then([callback](web::http::http_response response)
    {
        ResponseInfo responseInfo;
        if (response.status_code() == 200)
        {
            responseInfo.status = ResponseInfo::Status::Success;
            responseInfo.message = response.extract_utf8string().get();
            // Parse the response and fill the Coin object accordingly
            AccountResourceCoin acctResourceCoin = AccountResourceCoin::FromJson(nlohmann::json::parse(responseInfo.message));
            callback(acctResourceCoin.dataProp().coinProp(), responseInfo);
        }
        else if (response.status_code() == 404)
        {
            responseInfo.status = ResponseInfo::Status::NotFound;
            responseInfo.message = "Resource not found. " + response.extract_utf8string().get();

            AptosRESTModel::AccountResourceCoin::Coin coin;
            coin.value() = "0";
            callback(coin, responseInfo);
        }
        else
        {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = "Connection error. " + response.extract_utf8string().get();
            callback(AptosRESTModel::AccountResourceCoin::Coin(), responseInfo);
        }
    }).wait();
}

void RestClient::GetAccountResource(std::function<void(bool, long, std::string)> callback, const AccountAddress &accountAddress, const std::string &resourceType, const std::string &ledgerVersion)
{
    std::string uri = endpoint + "/v1/accounts/" + accountAddress.ToString() + "/resource/" + resourceType;
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::GET);
    request.headers().set_content_type(U("application/json"));
    client.request(request).then([callback](web::http::http_response response)
    {
        if (response.status_code() == 200) {
            callback(true, response.status_code(), response.extract_utf8string().get());
        }  else if (response.status_code() >= 400) {
            callback(false, response.status_code(), response.extract_utf8string().get());
        } else {
            callback(false, 0, "ERROR: Connection Error");
        }
    }).wait();
}

void RestClient::GetAccountResources(std::function<void(bool, long, std::string)> callback, const AccountAddress &accountAddress, const std::string &ledgerVersion)
{
    std::string uri = endpoint + "/v1/accounts/" + accountAddress.ToString() + "/resources";
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::GET);
    request.headers().set_content_type(U("application/json"));
    client.request(request).then([callback](web::http::http_response response)
    {
        if (response.status_code() == 200) {
            callback(true, response.status_code(), response.extract_utf8string().get());
        }  else if (response.status_code() >= 400) {
            callback(false, response.status_code(), response.extract_utf8string().get());
        } else {
            callback(false, 0, "ERROR: Connection Error");
        }
    }).wait();
}

void RestClient::GetAccountResourceCollection(std::function<void(std::shared_ptr<AptosRESTModel::ResourceCollection>, AptosRESTModel::ResponseInfo)> callback,
                                              const AccountAddress &accountAddress,
                                              const std::string &resourceType)
{
    using namespace AptosRESTModel;
    std::string uri = endpoint + "/v1/accounts/" + accountAddress.ToString() + "/resource/" + resourceType;
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::GET);
    request.headers().set_content_type(U("application/json"));
    client.request(request).then([callback](web::http::http_response response)
    {
        ResponseInfo responseInfo;
        if (response.status_code() == 200) {
            responseInfo.status = ResponseInfo::Status::Success;
            responseInfo.message = response.extract_utf8string().get();
            ResourceCollection acctResource = ResourceCollection::FromJson(nlohmann::json::parse(responseInfo.message));
            callback(std::make_shared<ResourceCollection>(acctResource), responseInfo);
        }  else if (response.status_code() >= 400) {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = "Account resource not found. " + response.extract_utf8string().get();
            callback(nullptr, responseInfo);
        } else {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = "Connection error. " + response.extract_utf8string().get();
            callback(nullptr, responseInfo);
        }
    }).wait();
}

void RestClient::GetTableItemCoin(std::function<void(std::shared_ptr<AptosRESTModel::AccountResourceCoin>, AptosRESTModel::ResponseInfo)> callback, const std::string &handle, const std::string &keyType, const std::string &valueType, const std::string &key)
{
    using namespace AptosRESTModel;
    std::string uri = endpoint + "/v1/tables/" + handle + "/item/";
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::GET);
    request.headers().set_content_type(U("application/json"));
    client.request(request).then([callback](web::http::http_response response)
    {
        ResponseInfo responseInfo;
        if (response.status_code() == 200) {
            responseInfo.status = ResponseInfo::Status::Success;
            responseInfo.message = response.extract_utf8string().get();
            AccountResourceCoin acctResource = AccountResourceCoin::FromJson(nlohmann::json::parse(responseInfo.message));
            callback(std::make_shared<AccountResourceCoin>(acctResource), responseInfo);
        }  else if (response.status_code() >= 400) {
            responseInfo.status = ResponseInfo::Status::NotFound;
            responseInfo.message = "Table item not found. " + response.extract_utf8string().get();
            callback(nullptr, responseInfo);
        } else {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = "Error while sending request for table item. " + response.extract_utf8string().get();
            callback(nullptr, responseInfo);
        }
    }).wait();
}

void RestClient::GetTableItem(std::function<void(std::string)> callback, const std::string &handle, const std::string &keyType, const std::string &valueType, const std::string &key)
{
    using namespace AptosRESTModel;
    TableItemRequest tableItemRequest;
    tableItemRequest.setKeyType(keyType);
    tableItemRequest.setValueType(valueType);
    tableItemRequest.setKey(key);
    std::string uri = endpoint + "/v1/tables/" + handle + "/item";
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::POST);
    request.headers().set_content_type(U("application/json"));
    request.set_body(tableItemRequest.ToJson().dump());
    client.request(request).then([callback](web::http::http_response response)
    {
        if (response.status_code() == 200) {
            callback(response.extract_utf8string().get());
        } else {
            callback(nullptr);
        }
    }).wait();
}

void RestClient::GetTableItemNFT(std::function<void(std::shared_ptr<AptosRESTModel::TableItemToken>, AptosRESTModel::ResponseInfo)> callback,
                                 const std::string &handle,
                                 const std::string &keyType,
                                 const std::string &valueType,
                                 AptosRESTModel::TokenIdRequest key)
{
    using namespace AptosRESTModel;
    TableItemRequestNFT tableItemRequest;
    tableItemRequest.setKeyType(keyType);
    tableItemRequest.setValueType(valueType);
    tableItemRequest.setKey(key);

    // Serialize the request object to JSON
    std::string tableItemRequestJson = tableItemRequest.ToJson().dump();
    std::string uri = endpoint + "/v1/tables/" + handle + "/item";
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::POST);
    request.headers().set_content_type(U("application/json"));
    request.set_body(tableItemRequestJson);
    client.request(request).then([callback,key](web::http::http_response response)
    {
        ResponseInfo responseInfo;
        TableItemToken tableItemToken;
        if (response.status_code() == 200) {
            responseInfo.status = ResponseInfo::Status::Success;
            responseInfo.message = response.extract_utf8string().get();
            tableItemToken = TableItemToken::FromJson(nlohmann::json::parse(responseInfo.message));
            callback(std::make_shared<TableItemToken>(tableItemToken), responseInfo);
        }  else if (response.status_code() >= 400) {
            tableItemToken.getIdProp().getTokenDataIdProp().setCreator(key.getTokenDataIdProp().getCreator());
            tableItemToken.getIdProp().getTokenDataIdProp().setCollection(key.getTokenDataIdProp().getCollection());
            tableItemToken.getIdProp().getTokenDataIdProp().setName(key.getTokenDataIdProp().getName());
            tableItemToken.setAmount("0");

            responseInfo.status = ResponseInfo::Status::NotFound;
            responseInfo.message = "Table item not found.";
            callback(std::make_shared<TableItemToken>(tableItemToken), responseInfo);
        } else {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = response.extract_utf8string().get();
            callback(nullptr, responseInfo);
        }
    }).wait();
}

void RestClient::GetTableItemTokenData(std::function<void(std::shared_ptr<AptosRESTModel::TableItemTokenMetadata>, AptosRESTModel::ResponseInfo)> callback, const std::string &handle, const std::string &keyType, const std::string &valueType, AptosRESTModel::TokenDataId key)
{
    using namespace AptosRESTModel;
    TableItemRequestTokenData tableItemRequest;
    tableItemRequest.setKey_type(keyType);
    tableItemRequest.setValue_type(valueType);
    tableItemRequest.setKey(key);

    // Serialize the request object to JSON
    std::string tableItemRequestJson = tableItemRequest.ToJson().dump();

    // Perform the HTTP request using the serialized JSON

    std::string uri = endpoint + "/v1/tables/" + handle + "/item";
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::POST);
    request.headers().set_content_type(U("application/json"));
    request.set_body(tableItemRequestJson);
   client.request(request).then([callback](web::http::http_response response)
    {
        ResponseInfo responseInfo;
        std::shared_ptr<TableItemTokenMetadata> tableItemToken = nullptr;
        if (response.status_code() >= 400) {
            responseInfo.status = ResponseInfo::Status::NotFound;
            responseInfo.message = "Table item not found.";
        } else {
            responseInfo.status = ResponseInfo::Status::Success;
            responseInfo.message = response.extract_utf8string().get();
            tableItemToken = std::make_shared<TableItemTokenMetadata>(
                TableItemTokenMetadata::FromJson(nlohmann::json::parse(responseInfo.message)));
        }
        callback(tableItemToken, responseInfo);
    }).wait();
}

void RestClient::GetInfo(std::function<void(std::shared_ptr<AptosRESTModel::LedgerInfo>, AptosRESTModel::ResponseInfo)> callback)
{
    using namespace AptosRESTModel;
    // Perform the HTTP request to get info
    std::string uri = endpoint + "/v1/";
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::GET);
    request.headers().set_content_type(U("application/json"));
    client.request(request).then([callback](web::http::http_response response)
    {
        ResponseInfo responseInfo;
        std::shared_ptr<LedgerInfo> ledgerInfo = nullptr;

        if (response.status_code() == 200) {
            responseInfo.status = ResponseInfo::Status::Success;
            responseInfo.message = response.extract_utf8string().get();
            ledgerInfo = std::make_shared<LedgerInfo>(LedgerInfo::FromJson(nlohmann::json::parse(responseInfo.message)));
        }  else if (response.status_code() >= 400) {
            responseInfo.status = ResponseInfo::Status::NotFound;
            responseInfo.message = "Resource not found.";
        } else {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = response.extract_utf8string().get();
        }
        callback(ledgerInfo, responseInfo);
    }).wait();
}

void RestClient::SimulateTransaction(std::function<void(std::string, AptosRESTModel::ResponseInfo)> callback,
                                     RawTransaction transaction, std::vector<uint8_t> publicKey)
{
    using namespace AptosRESTModel;
    CryptoPP::SecByteBlock byteBlock(Ed25519Signature::SignatureLength);
    std::memset(byteBlock.BytePtr(), 0, byteBlock.SizeInBytes());
    Ed25519Signature emptySignature(byteBlock);
    Authenticator authenticator{std::make_shared<Ed25519Authenticator>(PublicKey(Aptos::Utils::ByteVectorToSecBlock(publicKey)), emptySignature)};
    SignedTransaction signedTransaction(transaction, authenticator);

    // Perform the HTTP request to simulate a transaction
    std::string uri = endpoint + "/v1/transactions/simulate";
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::POST);
    request.headers().set_content_type(U("application/x.aptos.signed_transaction+bcs"));
    request.set_body(signedTransaction.Bytes());
    client.request(request).then([callback](web::http::http_response response)
    {
        ResponseInfo responseInfo;
        std::string res;
        if (response.status_code() == 200) {
            res = response.extract_utf8string().get();
            responseInfo.status = ResponseInfo::Status::Success;
            responseInfo.message = response.extract_utf8string().get();
        }  else if (response.status_code() >= 400) {
            responseInfo.status = ResponseInfo::Status::NotFound;
            responseInfo.message = "Endpoint not found.";
        } else {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = response.extract_utf8string().get();
        }
        callback(res, responseInfo);
    }).wait();
}

void RestClient::SubmitBCSTransaction(std::function<void(std::string, AptosRESTModel::ResponseInfo)> callback, const SignedTransaction &signedTransaction)
{
    using namespace AptosRESTModel;
    std::string uri = endpoint + "/v1/transactions";
    // Perform the request and handle the response
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::POST);
    request.headers().set_content_type(U("application/x.aptos.signed_transaction+bcs"));
    request.set_body(signedTransaction.Bytes());
    client.request(request).then([callback](web::http::http_response response)
    {
        ResponseInfo responseInfo;
        std::string responseStr;
        if (response.status_code() >= 400) {
            responseInfo.status = ResponseInfo::Status::NotFound;
            responseInfo.message = "Endpoint for BCS transaction not found.";
        } else {
            responseStr = response.extract_utf8string().get();
            responseInfo.status = ResponseInfo::Status::Success;
            responseInfo.message = responseStr;
        }
        callback(responseStr, responseInfo);
    }).wait();
}

void RestClient::View(std::function<void(std::vector<std::string>, AptosRESTModel::ResponseInfo)> callback,
                      const AptosRESTModel::ViewRequest &viewRequest)
{
    using namespace AptosRESTModel;
    std::string uri = endpoint + "/v1/view";
    // Perform the request and handle the response
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::POST);
    request.headers().set_content_type(U("application/json"));
    request.set_body(viewRequest.ToJson().dump());
    client.request(request).then([callback](web::http::http_response response)
    {
        ResponseInfo responseInfo;
        std::vector<std::string> values;
        if (response.status_code() == 200) {
            responseInfo.status = ResponseInfo::Status::Success;
            responseInfo.message = response.extract_utf8string().get();
            values = nlohmann::json::parse(responseInfo.message).get<std::vector<std::string>>();
        }  else {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = response.extract_utf8string().get();
        }
        callback(values, responseInfo);
    }).wait();
}

void RestClient::SubmitTransaction(std::function<void(std::shared_ptr<AptosRESTModel::Transaction>, AptosRESTModel::ResponseInfo)> callback,
                                   Account sender, EntryFunction entryFunction)
{
    using namespace AptosRESTModel;
    std::string sequenceNumber;
    ResponseInfo responseInfo;
    GetAccountSequenceNumber([&sequenceNumber, &responseInfo](std::string _sequenceNumber, ResponseInfo _responseInfo)
                             {
                                 sequenceNumber = _sequenceNumber;
                                 responseInfo = _responseInfo; },
                             *sender.getAccountAddress());

    if (responseInfo.status != ResponseInfo::Status::Success)
    {
        callback(nullptr, responseInfo);
        return;
    }
    std::time_t currentTimestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::time_t expirationTimestamp = currentTimestamp + ClientConfig::EXPIRATION_TTL;

    // Convert expiration timestamp to a string
    std::stringstream ss;
    ss << expirationTimestamp;
    std::string expirationTimestampStr = ss.str();

    TransactionRequest txnRequest;
    txnRequest.setSender(sender.getAccountAddress()->ToString());
    txnRequest.setSequenceNumber(sequenceNumber);
    txnRequest.setMaxGasAmount(std::to_string(ClientConfig::MAX_GAS_AMOUNT));
    txnRequest.setGasUnitPrice(std::to_string(ClientConfig::GAS_UNIT_PRICE));
    txnRequest.setExpirationTimestampSecs(expirationTimestampStr);
    txnRequest.setEntryFunction(entryFunction);
    auto txnRequestJson = TransactionRequestConverter::WriteJson(txnRequest);
    std::string encodedSubmission;
    EncodeSubmission([&encodedSubmission](const std::string &_encodedSubmission)
                     { encodedSubmission = _encodedSubmission; },
                     txnRequestJson.dump());
    std::vector<uint8_t> toSign = Aptos::Utils::ByteArrayFromHexString(Aptos::Utils::trim(encodedSubmission, "\"").substr(2));

    Ed25519Signature signature = sender.Sign(Aptos::Utils::ByteVectorToSecBlock(toSign));

    SignatureData sigData;
    sigData.setType(Constants::ED25519_SIGNATURE);
    sigData.setPublicKey(sender.getPublicKey()->ToString());
    sigData.setSignature(signature.ToString());
    txnRequest.setSignature(sigData);
    txnRequestJson = txnRequestJson = TransactionRequestConverter::WriteJson(txnRequest);

    std::string uri = endpoint + "/v1/transactions";
    // Perform the request and handle the response
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::POST);
    request.headers().set_content_type(U("application/json"));
    request.set_body(txnRequestJson.dump());
    client.request(request).then([callback](web::http::http_response response)
    {
        ResponseInfo responseInfo;
        std::vector<std::string> values;
        if (response.status_code() == 200 || response.status_code() == 202) {
            responseInfo.status = ResponseInfo::Status::Success;
            responseInfo.message = response.extract_utf8string().get();
            auto transaction = std::make_shared<Transaction>(
                TransactionConverter::ReadJson(nlohmann::json::parse(responseInfo.message)));
            callback(transaction, responseInfo);
        }  else {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = "Error in processing the response.";
            callback(nullptr, responseInfo);
        }
    }).wait();
}

void RestClient::WaitForTransaction(std::function<void(bool, AptosRESTModel::ResponseInfo)> callback, const std::string &txnHash)
{
    using namespace AptosRESTModel;
    int count = 0;            // Current attempt at querying for hash
    bool isTxnPending = true; // Has the transaction been confirmed in the blockchain

    bool isTxnSuccessful = false; // Was the transaction successful
    ResponseInfo responseInfo;
    responseInfo.status = ResponseInfo::Status::Failed;

    while (isTxnPending)
    {
        ResponseInfo responseInfoTxnPending;

        // Check if the transaction hash can be found
        TransactionPending([&isTxnPending, &responseInfoTxnPending](bool _isTxnPending, ResponseInfo _responseInfoTxnPending)
                           {
                               isTxnPending = _isTxnPending;
                               responseInfoTxnPending = _responseInfoTxnPending; },
                           txnHash);

        // If transaction hash has been found in the blockchain (not "pending"), check if it was successful
        if (!isTxnPending)
        {
            auto transaction = TransactionConverter::ReadJson(nlohmann::json::parse(responseInfoTxnPending.message));
            if (transaction.getSuccess())
            {
                responseInfo.status = ResponseInfo::Status::Success;
                responseInfo.message = responseInfoTxnPending.message;

                isTxnSuccessful = true;
                break;
            }

        }

        // Timeout if the transaction is still pending (hash not found) and we have queried N times
        // Set the boolean response to false, break -- we did not find the transaction
        if (count > Constants::TRANSACTION_WAIT_IN_SECONDS)
        {
            responseInfo.message = "Response Timed Out After Querying " + std::to_string(count) + " Times";
            isTxnSuccessful = false;
            break;
        }

        count += 1;
    }
    callback(isTxnSuccessful, responseInfo);
}

void RestClient::TransactionPending(std::function<void(bool, AptosRESTModel::ResponseInfo)> callback, const std::string &txnHash)
{
    using namespace AptosRESTModel;
    std::string uri = endpoint + "/v1/transactions/by_hash/" + txnHash;
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::GET);
    request.headers().set_content_type(U("application/json"));
    client.request(request).then([callback](web::http::http_response response)
    {
        ResponseInfo responseInfo;
        if (response.status_code() == 200) {
            responseInfo.status = ResponseInfo::Status::Success;
            responseInfo.message = response.extract_utf8string().get();
            auto transactionResult = TransactionConverter::ReadJson(nlohmann::json::parse(responseInfo.message));
            bool isPending = transactionResult.getType() == "pending_transaction";
            callback(isPending, responseInfo);
        }  else {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = "Transaction Call Error: " + std::to_string(response.status_code()) + " : " +
                                   response.extract_utf8string().get();

            callback(true, responseInfo);
        }
    }).wait();
}

void RestClient::TransactionByHash(std::function<void(AptosRESTModel::Transaction, AptosRESTModel::ResponseInfo)> callback, const std::string &txnHash)
{
    using namespace AptosRESTModel;
    std::string uri = endpoint + "/v1/transactions/by_hash/" + txnHash;
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::GET);
    request.headers().set_content_type(U("application/json"));
    client.request(request).then([callback](web::http::http_response response)
    {
        ResponseInfo responseInfo;
        if (response.status_code() == 200) {
            responseInfo.status = ResponseInfo::Status::Success;
            responseInfo.message = response.extract_utf8string().get();
            auto transactionResult = TransactionConverter::ReadJson(nlohmann::json::parse(responseInfo.message));
            callback(transactionResult, responseInfo);
        }  else {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = "Transaction Call Error: " + std::to_string(response.status_code()) + " : " +
                                   response.extract_utf8string().get();

            callback({}, responseInfo);
        }
    }).wait();
}

void RestClient::CreateBCSSignedTransaction(std::function<void(std::shared_ptr<SignedTransaction>)> Callback, Account Sender, TransactionPayload Payload)
{
    std::shared_ptr<RawTransaction> rawTransaction;
    CreateBCSTransaction([&rawTransaction](std::shared_ptr<RawTransaction> _rawTransaction)
                         { rawTransaction = _rawTransaction; },
                         Sender, Payload);

    Ed25519Signature signature = Sender.Sign(Aptos::Utils::ByteVectorToSecBlock(rawTransaction->Keyed()));
    Authenticator authenticator(std::make_shared<Ed25519Authenticator>(*Sender.getPublicKey(), signature));

    Callback(std::make_shared<SignedTransaction>(*rawTransaction, authenticator));
}

void RestClient::CreateBCSTransaction(std::function<void(std::shared_ptr<RawTransaction>)> Callback, Account Sender, TransactionPayload payload)
{
    using namespace AptosRESTModel;
    std::string sequenceNumber = "";
    ResponseInfo responseInfo;

    GetAccountSequenceNumber([&sequenceNumber, &responseInfo](std::string _sequenceNumber, ResponseInfo _responseInfo)
                             {
                                 sequenceNumber = _sequenceNumber;
                                 responseInfo = _responseInfo; },
                             *Sender.getAccountAddress());

    if (responseInfo.status != ResponseInfo::Status::Success) {
        throw std::runtime_error("Unable to get sequence number for: " + Sender.getAccountAddress()->ToString() + ".\n" + responseInfo.message);
    }

    auto currentTimePoint = std::chrono::system_clock::now();

    // Convert the time point to the duration since the epoch
    auto durationSinceEpoch = currentTimePoint.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch);
    std::time_t currentTime = seconds.count();
    unsigned long expirationTimestamp = static_cast<unsigned long>(currentTime + Constants::EXPIRATION_TTL);

    auto rawTxn = std::make_shared<RawTransaction>(
        *Sender.getAccountAddress(),
        std::stoi(sequenceNumber),
        payload,
        ClientConfig::MAX_GAS_AMOUNT,
        ClientConfig::GAS_UNIT_PRICE,
        expirationTimestamp,
        m_ChainId
        );

    Callback(rawTxn);
}

void RestClient::Transfer(std::function<void(std::shared_ptr<AptosRESTModel::Transaction>, AptosRESTModel::ResponseInfo)> callback, Account sender, const std::string &to, long amount)
{
    using namespace AptosRESTModel;
    if (!Aptos::Utils::IsValidAddress(to))
    {
        ResponseInfo responseInfo;
        responseInfo.status = ResponseInfo::Status::Failed;
        responseInfo.message = "Recipient address is invalid. " + to;
        callback(nullptr, responseInfo);
        return;
    }
    std::vector<std::shared_ptr<ISerializable>> sqD;
    sqD.push_back(std::make_shared<AccountAddress>(AccountAddress::FromHex(to)));
    sqD.push_back(std::make_shared<U64>(amount));

    EntryFunction payload(
        ModuleId(AccountAddress::FromHex("0x1"), "aptos_account"),
        "transfer",
        TagSequence(),
        Sequence(sqD));

    std::shared_ptr<Transaction> submitTxn;
    ResponseInfo responseInfo;
    SubmitTransaction([&submitTxn, &responseInfo](std::shared_ptr<Transaction> _submitTxn, ResponseInfo _responseInfo)
                      {
                          submitTxn = _submitTxn;
                          responseInfo = _responseInfo; },
                      sender, payload);

    if (responseInfo.status == ResponseInfo::Status::Success)
    {
        Transaction transaction = TransactionConverter::ReadJson(nlohmann::json::parse(responseInfo.message));
        callback(std::make_shared<Transaction>(transaction), responseInfo);
    }
    else
    {
        callback(nullptr, responseInfo);
    }
}

void RestClient::BCSTransfer(std::function<void(std::string, AptosRESTModel::ResponseInfo)> Callback, Account Sender, AccountAddress Recipient, int Amount)
{
    std::vector<std::shared_ptr<ISerializable>> transactionArguments;
    transactionArguments.push_back(std::make_shared<AccountAddress>(Recipient));
    transactionArguments.push_back(std::make_shared<U64>(Amount));

    EntryFunction payload = EntryFunction::Natural(
        ModuleId(AccountAddress::FromHex("0x1"), "aptos_account"),
        "transfer",
        TagSequence(),
        Sequence(transactionArguments));

    std::shared_ptr<SignedTransaction> signedTransaction = nullptr;
    CreateBCSSignedTransaction([&signedTransaction](std::shared_ptr<SignedTransaction> _signedTransaction)
                               { signedTransaction = _signedTransaction; },
                               Sender, TransactionPayload(std::make_shared<EntryFunction>(payload)));

    AptosRESTModel::ResponseInfo responseInfo;
    std::string submitBcsTxnJsonResponse = "";

    SubmitBCSTransaction([&responseInfo, &submitBcsTxnJsonResponse](std::string _responseJson,
                                                                    AptosRESTModel::ResponseInfo _responseInfo)
                         {
                             submitBcsTxnJsonResponse = _responseJson;
                             responseInfo = _responseInfo; },
                         *signedTransaction);
    Callback(submitBcsTxnJsonResponse, responseInfo);
}

void RestClient::EncodeSubmission(const std::function<void(const std::string &)> &callback, const std::string &txnRequestJson)
{
    std::string uri = endpoint + "/v1/transactions/encode_submission";
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::POST);
    request.set_body(txnRequestJson);
    request.headers().set_content_type(U("application/json"));
    client.request(request).then([callback](web::http::http_response response)
    {
        if (response.status_code() == 200) {
            callback(response.extract_utf8string().get());
        } else {
            std::cerr << "Error encode submission " << response.status_code() << " body " << response.extract_utf8string().get() << std::endl;
        }
    }).wait();
}

void RestClient::EncodeSubmissionAsBytes(const std::function<void(const std::vector<uint8_t> &)> &callback, const std::string &txnRequestJson)
{
    std::string uri = endpoint + "/v1/transactions/encode_submission";
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::POST);
    request.headers().set_content_type(U("application/json"));
    request.set_body(txnRequestJson);
    client.request(request).then([callback](web::http::http_response response)
    {
        if (response.status_code() == 200) {
            std::string msg = response.extract_utf8string().get();
            std::vector<uint8_t> byteVector(msg.begin(), msg.end());
            callback(byteVector);
        }
    }).wait();
}

void RestClient::CreateCollection(std::function<void(AptosRESTModel::Transaction, AptosRESTModel::ResponseInfo)> callback, Account sender, std::string collectionName, std::string collectionDescription, std::string uri)
{
    std::vector<std::shared_ptr<ISerializable>> transactionArguments;
    transactionArguments.push_back(std::make_shared<BString>(collectionName));
    transactionArguments.push_back(std::make_shared<BString>(collectionDescription));
    transactionArguments.push_back(std::make_shared<BString>(uri));
    transactionArguments.push_back(std::make_shared<U64>(18446744073709551615UL));
    std::vector<std::shared_ptr<ISerializable>> t2;
    t2.push_back(std::make_shared<Bool>(false));
    t2.push_back(std::make_shared<Bool>(false));
    t2.push_back(std::make_shared<Bool>(false));
    transactionArguments.push_back(std::make_shared<Sequence>(t2));

    EntryFunction payload = EntryFunction::Natural(
        ModuleId(AccountAddress::FromHex("0x3"), "token"),
        "create_collection_script",
        TagSequence(),
        Sequence(transactionArguments));

    TransactionPayload txnPayload(std::make_shared<EntryFunction>(payload));

    std::shared_ptr<SignedTransaction> signedTransaction = nullptr;

    CreateBCSSignedTransaction([&signedTransaction](std::shared_ptr<SignedTransaction> _signedTransaction)
                               { signedTransaction = _signedTransaction; },
                               sender, txnPayload);

    std::string submitBcsTxnJsonResponse;
    AptosRESTModel::ResponseInfo responseInfo;

    SubmitBCSTransaction([&submitBcsTxnJsonResponse, &responseInfo](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
                         {
                             submitBcsTxnJsonResponse = _responseJson;
                             responseInfo = _responseInfo; },
                         *signedTransaction);
    AptosRESTModel::Transaction createCollectionTxn;
    createCollectionTxn = AptosRESTModel::TransactionConverter::ReadJson(nlohmann::json::parse(submitBcsTxnJsonResponse));
    callback(createCollectionTxn, responseInfo);
}

void RestClient::CreateToken(std::function<void(AptosRESTModel::Transaction, AptosRESTModel::ResponseInfo)> callback, Account senderRoyaltyPayeeAddress, std::string collectionName, std::string tokenName, std::string description, int supply, int max, std::string uri, int royaltyPointsPerMillion)
{
    std::vector<std::shared_ptr<ISerializable>> transactionArguments;
    transactionArguments.push_back(std::make_shared<BString>(collectionName));
    transactionArguments.push_back(std::make_shared<BString>(tokenName));
    transactionArguments.push_back(std::make_shared<BString>(description));
    transactionArguments.push_back(std::make_shared<U64>(supply));
    transactionArguments.push_back(std::make_shared<U64>(supply));
    transactionArguments.push_back(std::make_shared<BString>(uri));
    transactionArguments.push_back(senderRoyaltyPayeeAddress.getAccountAddress());
    transactionArguments.push_back(std::make_shared<U64>(1000000));
    transactionArguments.push_back(std::make_shared<U64>(royaltyPointsPerMillion));
    std::vector<std::shared_ptr<ISerializable>> t2;
    t2.push_back(std::make_shared<Bool>(false));
    t2.push_back(std::make_shared<Bool>(false));
    t2.push_back(std::make_shared<Bool>(false));
    t2.push_back(std::make_shared<Bool>(false));
    t2.push_back(std::make_shared<Bool>(false));
    transactionArguments.push_back(std::make_shared<Sequence>(t2));
    std::vector<std::shared_ptr<ISerializable>> t3;
    std::vector<std::shared_ptr<ISerializable>> t4;
    std::vector<std::shared_ptr<ISerializable>> t5;
    transactionArguments.push_back(std::make_shared<Sequence>(t3));
    transactionArguments.push_back(std::make_shared<Sequence>(t4));
    transactionArguments.push_back(std::make_shared<Sequence>(t5));

    EntryFunction payload = EntryFunction::Natural(
        ModuleId(AccountAddress::FromHex("0x3"), "token"),
        "create_token_script",
        TagSequence(),
        Sequence(transactionArguments));

    TransactionPayload txnPayload(std::make_shared<EntryFunction>(payload));

    std::shared_ptr<SignedTransaction> signedTransaction = nullptr;

    CreateBCSSignedTransaction([&signedTransaction](std::shared_ptr<SignedTransaction> _signedTransaction)
                               { signedTransaction = _signedTransaction; },
                               senderRoyaltyPayeeAddress, txnPayload);

    std::string submitBcsTxnJsonResponse;
    AptosRESTModel::ResponseInfo responseInfo;

    SubmitBCSTransaction([&submitBcsTxnJsonResponse, &responseInfo](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
                         {
                             submitBcsTxnJsonResponse = _responseJson;
                             responseInfo = _responseInfo; },
                         *signedTransaction);

    AptosRESTModel::Transaction createTokenTxn;
    createTokenTxn = AptosRESTModel::TransactionConverter::ReadJson(nlohmann::json::parse(submitBcsTxnJsonResponse));
    callback(createTokenTxn, responseInfo);
}

void RestClient::OfferToken(std::function<void(std::shared_ptr<AptosRESTModel::Transaction>, AptosRESTModel::ResponseInfo)> callback, Account account, AccountAddress receiver, AccountAddress creator, std::string collectionName, std::string tokenName, int amount, int propertyVersion)
{
    AptosRESTModel::ResponseInfo responseInfo;
    if (!Aptos::Utils::IsValidAddress(receiver.ToString()))
    {
        responseInfo.status = AptosRESTModel::ResponseInfo::Status::Failed;
        responseInfo.message = "Recipient address is invalid.";
        callback(nullptr, responseInfo);
        return;
    }
    if (!Aptos::Utils::IsValidAddress(creator.ToString()))
    {
        responseInfo.status = AptosRESTModel::ResponseInfo::Status::Failed;
        responseInfo.message = "Creator address is invalid.";
        callback(nullptr, responseInfo);
        return;
    }
    std::vector<std::shared_ptr<ISerializable>> transactionArguments;
    transactionArguments.push_back(std::make_shared<AccountAddress>(receiver));
    transactionArguments.push_back(std::make_shared<AccountAddress>(creator));
    transactionArguments.push_back(std::make_shared<BString>(collectionName));
    transactionArguments.push_back(std::make_shared<BString>(tokenName));
    transactionArguments.push_back(std::make_shared<U64>(propertyVersion));
    transactionArguments.push_back(std::make_shared<U64>(amount));

    EntryFunction payload = EntryFunction::Natural(
        ModuleId(AccountAddress::FromHex("0x3"), "token_transfers"),
        "offer_script",
        TagSequence(),
        Sequence(transactionArguments));

    TransactionPayload txnPayload(std::make_shared<EntryFunction>(payload));

    std::shared_ptr<SignedTransaction> signedTransaction = nullptr;

    CreateBCSSignedTransaction([&signedTransaction](std::shared_ptr<SignedTransaction> _signedTransaction)
                               { signedTransaction = _signedTransaction; },
                               account, txnPayload);

    std::string submitBcsTxnJsonResponse;

    SubmitBCSTransaction([&submitBcsTxnJsonResponse, &responseInfo](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
                         {
                             submitBcsTxnJsonResponse = _responseJson;
                             responseInfo = _responseInfo; },
                         *signedTransaction);

    AptosRESTModel::Transaction offerTokenTxn;
    offerTokenTxn = AptosRESTModel::TransactionConverter::ReadJson(nlohmann::json::parse(submitBcsTxnJsonResponse));
    callback(std::make_shared<AptosRESTModel::Transaction>(offerTokenTxn), responseInfo);
}

void RestClient::ClaimToken(std::function<void(std::shared_ptr<AptosRESTModel::Transaction>, AptosRESTModel::ResponseInfo)> callback, Account account, AccountAddress sender, AccountAddress creator, std::string collectionName, std::string tokenName, int propertyVersion)
{
    AptosRESTModel::ResponseInfo responseInfo;
    if (!Aptos::Utils::IsValidAddress(sender.ToString()))
    {
        responseInfo.status = AptosRESTModel::ResponseInfo::Status::Failed;
        responseInfo.message = "Sender address is invalid.";
        callback(nullptr, responseInfo);
        return;
    }
    if (!Aptos::Utils::IsValidAddress(creator.ToString()))
    {
        responseInfo.status = AptosRESTModel::ResponseInfo::Status::Failed;
        responseInfo.message = "Creator address is invalid.";
        callback(nullptr, responseInfo);
        return;
    }

    std::vector<std::shared_ptr<ISerializable>> transactionArguments;
    transactionArguments.push_back(std::make_shared<AccountAddress>(sender));
    transactionArguments.push_back(std::make_shared<AccountAddress>(creator));
    transactionArguments.push_back(std::make_shared<BString>(collectionName));
    transactionArguments.push_back(std::make_shared<BString>(tokenName));
    transactionArguments.push_back(std::make_shared<U64>(propertyVersion));

    EntryFunction payload = EntryFunction::Natural(
        ModuleId(AccountAddress::FromHex("0x3"), "token_transfers"),
        "claim_script",
        TagSequence(),
        Sequence(transactionArguments));

    TransactionPayload txnPayload(std::make_shared<EntryFunction>(payload));

    std::shared_ptr<SignedTransaction> signedTransaction = nullptr;

    CreateBCSSignedTransaction([&signedTransaction](std::shared_ptr<SignedTransaction> _signedTransaction)
                               { signedTransaction = _signedTransaction; },
                               account, txnPayload);

    std::string submitBcsTxnJsonResponse;

    SubmitBCSTransaction([&submitBcsTxnJsonResponse, &responseInfo](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
                         {
                             submitBcsTxnJsonResponse = _responseJson;
                             responseInfo = _responseInfo; },
                         *signedTransaction);

    AptosRESTModel::Transaction claimTokenTxn;
    claimTokenTxn = AptosRESTModel::TransactionConverter::ReadJson(nlohmann::json::parse(submitBcsTxnJsonResponse));
    callback(std::make_shared<AptosRESTModel::Transaction>(claimTokenTxn), responseInfo);
}

void RestClient::GetToken(std::function<void(AptosRESTModel::TableItemToken, AptosRESTModel::ResponseInfo)> callback, AccountAddress ownerAddress, AccountAddress creatorAddress, std::string collectionName, std::string tokenName, int propertyVersion)
{
    bool success = false;
    long responseCode = 0;
    std::string tokenStoreResourceResp;

    GetAccountResource([&success, &responseCode, &tokenStoreResourceResp](bool _success, long _responseCode, std::string _returnResult)
                       {
                           success = _success;
                           responseCode = _responseCode;
                           tokenStoreResourceResp = _returnResult; },
                       ownerAddress, "0x3::token::TokenStore");

    AptosRESTModel::ResponseInfo responseInfo;

    if (!success)
    {
        if (responseCode == 404)
        {
            responseInfo.status = AptosRESTModel::ResponseInfo::Status::NotFound;
        }
        else
        {
            responseInfo.status = AptosRESTModel::ResponseInfo::Status::Failed;
        }
        responseInfo.message = tokenStoreResourceResp;
        callback(AptosRESTModel::TableItemToken(), responseInfo);
        return;
    }

    AptosRESTModel::AccountResourceTokenStore accountResource =
        AptosRESTModel::AccountResourceTokenStore::FromJson(nlohmann::json::parse(tokenStoreResourceResp));
    std::string tokenStoreHandle = accountResource.getDataProp().getTokensProp().getHandle();

    AptosRESTModel::TokenIdRequest tokenId;
    tokenId.setPropertyVersion(std::to_string(propertyVersion));
    AptosRESTModel::TokenDataId tokenDataId;
    tokenDataId.setCreator(creatorAddress.ToString());
    tokenDataId.setCollection(collectionName);
    tokenDataId.setName(tokenName);
    tokenId.setTokenDataIdProp(tokenDataId);
    std::string tokenIdJson = tokenId.ToJson().dump();

    std::shared_ptr<AptosRESTModel::TableItemToken> tableItemToken;
    GetTableItemNFT([&tableItemToken, &responseInfo](std::shared_ptr<AptosRESTModel::TableItemToken> _tableItemToken, AptosRESTModel::ResponseInfo _responseInfo)
                    {
                        tableItemToken = _tableItemToken;
                        responseInfo = _responseInfo; },
                    tokenStoreHandle, "0x3::token::TokenId", "0x3::token::Token", tokenId);

    callback(*tableItemToken, responseInfo);
}

void RestClient::GetTokenBalance(std::function<void(std::string)> callback, AccountAddress ownerAddress, AccountAddress creatorAddress, std::string collectionName, std::string tokenName, int propertyVersion)
{
    AptosRESTModel::TableItemToken tableItemToken;
    AptosRESTModel::ResponseInfo responseInfo;
    GetToken([&tableItemToken, &responseInfo](AptosRESTModel::TableItemToken _tableItemToken, AptosRESTModel::ResponseInfo _responseInfo)
             {
                 tableItemToken = _tableItemToken;
                 responseInfo = _responseInfo; },
             ownerAddress, creatorAddress, collectionName, tokenName, propertyVersion);

    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Success)
    {
        std::string tokenBalance = tableItemToken.getAmount();
        callback(tokenBalance);
    }
    else
    {
        callback("0");
    }
}

void RestClient::GetTokenData(std::function<void(std::shared_ptr<AptosRESTModel::TableItemTokenMetadata>, AptosRESTModel::ResponseInfo)> callback, AccountAddress creator, std::string collectionName, std::string tokenName, int propertyVersion)
{
    bool success = false;
    long responseCode = 0;
    std::string collectionResourceResp;

    GetAccountResource([&success, &responseCode, &collectionResourceResp](bool _success, long _responseCode, std::string _returnResult)
                       {
                           success = _success;
                           responseCode = _responseCode;
                           collectionResourceResp = _returnResult; },
                       creator, "0x3::token::Collections");

    AptosRESTModel::ResponseInfo responseInfo;

    if (!success)
    {
        responseInfo.status = AptosRESTModel::ResponseInfo::Status::Failed;
        responseInfo.message = collectionResourceResp;
        callback(nullptr, responseInfo);
    }

    AptosRESTModel::ResourceCollection resourceCollection = AptosRESTModel::ResourceCollection::FromJson(nlohmann::json::parse(collectionResourceResp));
    std::string tokenDataHandle = resourceCollection.getDataProp().getTokenDataProp().getHandle();

    AptosRESTModel::TokenDataId tokenDataId;
    tokenDataId.setCreator(creator.ToString());
    tokenDataId.setCollection(collectionName);
    tokenDataId.setName(tokenName);
    std::string tokenDataIdJson = tokenDataId.ToJson().dump();

    std::shared_ptr<AptosRESTModel::TableItemTokenMetadata> tableItemTokenMetadata;

    GetTableItemTokenData([&tableItemTokenMetadata, &responseInfo](std::shared_ptr<AptosRESTModel::TableItemTokenMetadata> _tableItemTokenMetadata, AptosRESTModel::ResponseInfo _responseInfo)
                          {
                              tableItemTokenMetadata = _tableItemTokenMetadata;
                              responseInfo = _responseInfo; },
                          tokenDataHandle, "0x3::token::TokenDataId", "0x3::token::TokenData", tokenDataId);

    callback(tableItemTokenMetadata, responseInfo);
}

void RestClient::GetCollection(std::function<void(std::string)> callback, AccountAddress creator, std::string collectionName)
{
    bool success = false;
    long responseCode = 0;
    std::string collectionResourceResp;

    GetAccountResource([&success, &responseCode, &collectionResourceResp](bool _success, long _responseCode, std::string _returnResult)
                       {
                           success = _success;
                           responseCode = _responseCode;
                           collectionResourceResp = _returnResult; },
                       creator, "0x3::token::Collections");

    if (!success)
    {
        callback("Account resource not found");
        return;
    }

    AptosRESTModel::ResourceCollection resourceCollection = AptosRESTModel::ResourceCollection::FromJson(nlohmann::json::parse(collectionResourceResp));
    std::string tokenDataHandle = resourceCollection.getDataProp().getCollectionDataProp().getHandle();

    std::string tableItemResp;

    GetTableItem([&tableItemResp](std::string _returnResult)
                 { tableItemResp = _returnResult; },
                 tokenDataHandle, "0x1::string::String", "0x3::token::CollectionData", collectionName);

    callback(tableItemResp);
}

void RestClient::TransferObject(std::function<void(std::string, AptosRESTModel::ResponseInfo)> callback, Account Owner, AccountAddress Object, AccountAddress To)
{
    std::vector<std::shared_ptr<ISerializable>> transactionArguments;
    transactionArguments.push_back(std::make_shared<AccountAddress>(Object));
    transactionArguments.push_back(std::make_shared<AccountAddress>(To));

    EntryFunction payload = EntryFunction::Natural(
        ModuleId(AccountAddress::FromHex("0x1"), "object"),
        Constants::APTOS_TRANSFER_CALL,
        TagSequence(),
        Sequence(transactionArguments));

    std::shared_ptr<SignedTransaction> signedTransaction = nullptr;
    AptosRESTModel::ResponseInfo responseInfo;
    TransactionPayload txnPayload(std::make_shared<EntryFunction>(payload));

    CreateBCSSignedTransaction([&signedTransaction](std::shared_ptr<SignedTransaction> _signedTransaction)
                               { signedTransaction = _signedTransaction; },
                               Owner, txnPayload);

    std::string submitBcsTxnJsonResponse = "";

    SubmitBCSTransaction([&submitBcsTxnJsonResponse, &responseInfo](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
                         {
                             submitBcsTxnJsonResponse = _responseJson;
                             responseInfo = _responseInfo; },
                         *signedTransaction);
    callback(submitBcsTxnJsonResponse, responseInfo);
}

void RestClient::GetAccountResource(std::function<void(bool, long, std::string)> callback, AccountAddress &accountAddress, std::string resourceType)
{
    std::string uri = endpoint + "/v1/accounts/" + accountAddress.ToString() + "/resource/" + resourceType;
    web::http::client::http_client client(utility::conversions::to_string_t(uri));
    web::http::http_request request(web::http::methods::GET);
    request.headers().set_content_type(U("application/json"));
    client.request(request).then([callback](web::http::http_response response)
    {
        if (response.status_code() == 200) {
            callback(true, response.status_code(), response.extract_utf8string().get());
        } else {
            callback(false, response.status_code(), response.extract_utf8string().get());
        }
    }).wait();
}

int RestClient::ChainId() const
{
    return m_ChainId;
}
}