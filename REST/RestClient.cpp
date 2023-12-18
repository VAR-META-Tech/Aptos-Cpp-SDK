#include "RestClient.h"
#include "Model/TableItemRequest.h"
#include "Model/TableItemRequestNFT.h"
#include "Model/TableItemRequestTokenData.h"
#include "../BCS/Authenticator.h"
#include "../BCS/SignedTransaction.h"
#include "../HDWallet/Utils/Utils.h"
#include "Constant.h"
#include <thread>
#include "../BCS/ModuleId.h"
#include "../BCS/EntryFunction.h"
#include "../BCS/Sequence.h"
#include "../BCS/TagSequence.h"
#include "../BCS/U64.h"
#include "../BCS/Bool.h"
#include "../BCS/Bytes.h"
#include "Model/AccountResourceTokenStore.h"
#include "RequestClient.h"

namespace Aptos::Rest
{
    void RestClient::SetEndpoint(const std::string &url)
    {
        endpoint = url;
    }

    RestClient::RestClient() {}

    void RestClient::GetAccount(std::function<void(std::shared_ptr<AptosRESTModel::AccountData>, AptosRESTModel::ResponseInfo)> callback, const AccountAddress &accountAddress)
    {
        using namespace AptosRESTModel;

        std::string uri = "/v1/accounts/" + accountAddress.ToString();
        ResponseInfo responseInfo;
        if (auto response = RequestClient::GetWebClient(endpoint).Get(uri))
        {
            if (response->status == 200)
            {
                responseInfo.status = ResponseInfo::Status::Success;
                responseInfo.message = response->body;
                AccountData accountData = AccountData::FromJson(response->body);

                callback(std::make_shared<AccountData>(accountData), responseInfo);
            }
            else if (response->status == 404)
            {
                responseInfo.status = ResponseInfo::Status::NotFound;
                responseInfo.message = "Account not found";
                callback(nullptr, responseInfo);
            }
            else
            {
                responseInfo.status = ResponseInfo::Status::Failed;
                responseInfo.message = response->body;
                callback(nullptr, responseInfo);
            }
        }
        else
        {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = httplib::to_string(response.error());
            callback(nullptr, responseInfo);
        }
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

        if (responseInfo.status != ResponseInfo::Status::Success || accountData)
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
        std::string uri = "/v1/accounts/" + accountAddress.ToString() + "/resource/" + Constants::APTOS_COIN_TYPE;
        ResponseInfo responseInfo;
        if (auto response = RequestClient::GetWebClient(endpoint).Get(uri))
        {

            if (response->status == 200)
            {
                responseInfo.status = ResponseInfo::Status::Success;
                responseInfo.message = response->body;

                // Parse the response and fill the Coin object accordingly
                AccountResourceCoin acctResourceCoin = AccountResourceCoin::FromJson(response->body);\

                callback(acctResourceCoin.dataProp().coinProp(), responseInfo);
            }
            else if (response->status == 404)
            {
                responseInfo.status = ResponseInfo::Status::NotFound;
                responseInfo.message = "Resource not found. " + response->body;

                AptosRESTModel::AccountResourceCoin::Coin coin;
                coin.value() = "0";
                callback(coin, responseInfo);
            }
            else
            {
                responseInfo.status = ResponseInfo::Status::Failed;
                responseInfo.message = "Connection error. " + response->body;
                callback(AptosRESTModel::AccountResourceCoin::Coin(), responseInfo);
            }
        }
        else
        {
            responseInfo.status = ResponseInfo::Status::Failed;
            responseInfo.message = httplib::to_string(response.error());
            callback(AptosRESTModel::AccountResourceCoin::Coin(), responseInfo);
        }
    }

    void RestClient::GetAccountResource(std::function<void(bool, long, std::string)> callback, const AccountAddress &accountAddress, const std::string &resourceType, const std::string &ledgerVersion)
    {
        // web::uri_builder builder(endpoint);
        // builder.append_path(U("/accounts/")).append_path(utility::conversions::to_string_t(accountAddress)).append_path(U("/resource/")).append_path(utility::conversions::to_string_t(resourceType));
        // web::uri accountsURI = builder.to_uri();

        // web::http::client::http_client client(accountsURI);

        // client.request(web::http::methods::GET).then([=](web::http::http_response response)
        //                                              {
        //                                        if (response.status_code() == web::http::status_codes::OK) {
        //                                            callback(true, response.status_code(), utility::conversions::to_utf8string(response.to_string()));
        //                                        } else if (response.status_code() == web::http::status_codes::NotFound || response.status_code() >= 400) {
        //                                            callback(false, response.status_code(), utility::conversions::to_utf8string(response.to_string()));
        //                                        } else {
        //                                            callback(false, 0, "ERROR: Connection Error");
        //                                        } })
        //     .wait();
    }

    void RestClient::GetAccountResources(std::function<void(bool, long, std::string)> callback, const AccountAddress &accountAddress, const std::string &ledgerVersion)
    {
        // web::uri_builder builder(endpoint);
        // builder.append_path(U("/accounts/")).append_path(utility::conversions::to_string_t(accountAddress)).append_path(U("/resources"));
        // web::uri accountsURI = builder.to_uri();

        // web::http::client::http_client client(accountsURI);

        // client.request(web::http::methods::GET).then([=](web::http::http_response response)
        //                                              {
        //                                        if (response.status_code() == web::http::status_codes::OK)
        //                                        {
        //                                            callback(true, response.status_code(), utility::conversions::to_utf8string(response.to_string()));
        //                                        }
        //                                        else if (response.status_code() == web::http::status_codes::NotFound || response.status_code() >= 400)
        //                                        {
        //                                            callback(false, response.status_code(), utility::conversions::to_utf8string(response.to_string()));
        //                                        }
        //                                        else
        //                                        {
        //                                            callback(false, 0, "ERROR: Connection Error");
        //                                        } })
        //     .wait();
    }

    void RestClient::GetAccountResourceCollection(std::function<void(std::shared_ptr<AptosRESTModel::ResourceCollection>, AptosRESTModel::ResponseInfo)> callback,
                                                  const AccountAddress &accountAddress,
                                                  const std::string &resourceType)
    {
        // using namespace AptosRESTModel;
        // web::uri_builder builder(endpoint);
        // builder.append_path(U("/accounts/")).append_path(utility::conversions::to_string_t(accountAddress)).append_path(U("/resource/")).append_path(utility::conversions::to_string_t(resourceType));
        // web::uri accountsURI = builder.to_uri();
        // web::http::client::http_client client(accountsURI);
        // client.request(web::http::methods::GET).then([=](web::http::http_response response)
        //                                              {
        //                                        ResponseInfo responseInfo;
        //                                        if (response.status_code() == web::http::status_codes::OK)
        //                                        {
        //                                            ResourceCollection acctResource = ResourceCollection::FromJson(response.to_string());
        //                                            responseInfo.status = ResponseInfo::Status::Success;
        //                                            responseInfo.message = utility::conversions::to_utf8string(response.to_string());
        //                                            callback(std::make_shared<ResourceCollection>(acctResource), responseInfo);
        //                                        }
        //                                        else if (response.status_code() == web::http::status_codes::NotFound || response.status_code() >= 400)
        //                                        {
        //                                            responseInfo.status = ResponseInfo::Status::Failed;
        //                                            responseInfo.message = "Account resource not found. " + utility::conversions::to_utf8string(response.to_string());
        //                                            callback(nullptr, responseInfo);
        //                                        }
        //                                        else
        //                                        {
        //                                            responseInfo.status = ResponseInfo::Status::Failed;
        //                                            responseInfo.message = "Connection error. " + utility::conversions::to_utf8string(response.to_string());
        //                                            callback(nullptr, responseInfo);
        //                                        } })
        //     .wait();
    }

    void RestClient::GetTableItemCoin(std::function<void(std::shared_ptr<AptosRESTModel::AccountResourceCoin>, AptosRESTModel::ResponseInfo)> callback, const std::string &handle, const std::string &keyType, const std::string &valueType, const std::string &key)
    {
        // using namespace AptosRESTModel;
        // web::uri_builder builder(endpoint);
        // builder.append_path(U("/tables/")).append_path(utility::conversions::to_string_t(handle)).append_path(U("/item/"));
        // web::uri getTableItemURI = builder.to_uri();
        // web::http::client::http_client client(getTableItemURI);
        // client.request(web::http::methods::GET).then([=](web::http::http_response response)
        //                                              {
        //                                        ResponseInfo responseInfo;
        //                                        if (response.status_code() == web::http::status_codes::OK)
        //                                        {
        //                                            AccountResourceCoin acctResource = AccountResourceCoin::FromJson(response.to_string());
        //                                            responseInfo.status = ResponseInfo::Status::Success;
        //                                            responseInfo.message = utility::conversions::to_utf8string(response.to_string());
        //                                            callback(std::make_shared<AccountResourceCoin>(acctResource), responseInfo);
        //                                        }
        //                                        else if (response.status_code() == web::http::status_codes::NotFound)
        //                                        {
        //                                            responseInfo.status = ResponseInfo::Status::NotFound;
        //                                            responseInfo.message = "Table item not found. " + utility::conversions::to_utf8string(response.to_string());
        //                                            callback(nullptr, responseInfo);
        //                                        }
        //                                        else
        //                                        {
        //                                            responseInfo.status = ResponseInfo::Status::Failed;
        //                                            responseInfo.message = "Error while sending request for table item. " + utility::conversions::to_utf8string(response.to_string());
        //                                            callback(nullptr, responseInfo);
        //                                        } })
        //     .wait();
    }

    void RestClient::GetTableItem(std::function<void(std::string)> callback, const std::string &handle, const std::string &keyType, const std::string &valueType, const std::string &key)
    {
        // using namespace AptosRESTModel;
        // TableItemRequest tableItemRequest;
        // tableItemRequest.setKeyType(keyType);
        // tableItemRequest.setValueType(valueType);
        // tableItemRequest.setKey(key);

        // web::uri_builder builder(endpoint);
        // builder.append_path(U("/tables/")).append_path(utility::conversions::to_string_t(handle)).append_path(U("/item"));
        // web::uri getTableItemURI = builder.to_uri();

        // web::http::client::http_client client(getTableItemURI);

        // web::http::http_request request(web::http::methods::POST);
        // request.headers().set_content_type(U("application/json"));
        // request.set_body(web::json::value::string(utility::conversions::to_string_t(tableItemRequest.ToJson().dump())));

        // client.request(request).then([=](web::http::http_response response)
        //                              {
        //                        if (response.status_code() == web::http::status_codes::OK)
        //                        {
        //                            callback(utility::conversions::to_utf8string(response.to_string()));
        //                        }
        //                        else
        //                        {
        //                            callback(nullptr);
        //                        } })
        //     .wait();
    }

    void RestClient::GetTableItemNFT(std::function<void(std::shared_ptr<AptosRESTModel::TableItemToken>, AptosRESTModel::ResponseInfo)> callback,
                                     const std::string &handle,
                                     const std::string &keyType,
                                     const std::string &valueType,
                                     AptosRESTModel::TokenIdRequest key)
    {
        // using namespace AptosRESTModel;
        // TableItemRequestNFT tableItemRequest;
        // tableItemRequest.setKeyType(keyType);
        // tableItemRequest.setValueType(valueType);
        // tableItemRequest.setKey(key);

        // // Serialize the request object to JSON
        // std::string tableItemRequestJson = utility::conversions::to_utf8string(tableItemRequest.ToJson().dump());

        // // Perform the HTTP request using the serialized JSON
        // web::http::client::http_client client(endpoint);
        // web::uri_builder builder(U("/tables/" + handle + "/item"));

        // client.request(web::http::methods::POST, builder.to_string(), U(tableItemRequestJson), U("application/json"))
        //     .then([=](web::http::http_response response)
        //           {
        //     ResponseInfo responseInfo;
        //     TableItemToken tableItemToken;
        //     if (response.status_code() == web::http::status_codes::OK)
        //     {
        //         tableItemToken = TableItemToken::FromJson(response.to_string());
        //         responseInfo.status = ResponseInfo::Status::Success;
        //         responseInfo.message = utility::conversions::to_utf8string(response.to_string());
        //         callback(std::make_shared<TableItemToken>(tableItemToken), responseInfo);
        //     }
        //     else if (response.status_code() == web::http::status_codes::NotFound)
        //     {
        //         tableItemToken.getIdProp().getTokenDataIdProp().setCreator(key.getTokenDataIdProp().getCreator());
        //         tableItemToken.getIdProp().getTokenDataIdProp().setCollection(key.getTokenDataIdProp().getCollection());
        //         tableItemToken.getIdProp().getTokenDataIdProp().setName(key.getTokenDataIdProp().getName());
        //         tableItemToken.setAmount("0");

        //         responseInfo.status = ResponseInfo::Status::NotFound;
        //         responseInfo.message = "Table item not found.";
        //         callback(std::make_shared<TableItemToken>(tableItemToken), responseInfo);
        //     }  else if (response.status_code() == web::http::status_codes::BadRequest)
        //     {
        //         responseInfo.status = ResponseInfo::Status::Failed;
        //         responseInfo.message = utility::conversions::to_utf8string(response.to_string());

        //         callback(nullptr, responseInfo);
        //     } else {
        //         tableItemToken = TableItemToken::FromJson(response.to_string());
        //         responseInfo.status = ResponseInfo::Status::Success;
        //         responseInfo.message = utility::conversions::to_utf8string(response.to_string());
        //         callback(std::make_shared<TableItemToken>(tableItemToken), responseInfo);
        //     } })
        //     .wait();
    }

    void RestClient::GetTableItemTokenData(std::function<void(std::shared_ptr<AptosRESTModel::TableItemTokenMetadata>, AptosRESTModel::ResponseInfo)> callback, const std::string &handle, const std::string &keyType, const std::string &valueType, AptosRESTModel::TokenDataId key)
    {
        // using namespace AptosRESTModel;
        // TableItemRequestTokenData tableItemRequest;
        // tableItemRequest.setKey_type(keyType);
        // tableItemRequest.setValue_type(valueType);
        // tableItemRequest.setKey(key);

        // // Serialize the request object to JSON
        // std::string tableItemRequestJson = utility::conversions::to_utf8string(tableItemRequest.ToJson().dump());

        // // Perform the HTTP request using the serialized JSON
        // web::http::client::http_client client(endpoint);
        // web::uri_builder builder(U("/tables/" + handle + "/item"));

        // client.request(web::http::methods::POST, builder.to_string(), U(tableItemRequestJson), U("application/json"))
        //     .then([=](pplx::task<web::http::http_response> task)
        //           {
        //     ResponseInfo responseInfo;
        //     std::shared_ptr<TableItemTokenMetadata> tableItemToken = nullptr;
        //     try
        //     {
        //         web::http::http_response response = task.get();

        //         if (response.status_code() == web::http::status_codes::OK)
        //         {
        //             // Deserialize the JSON response
        //             tableItemToken = std::make_shared<TableItemTokenMetadata>(TableItemTokenMetadata::FromJson(response.to_string()));
        //             responseInfo.status = ResponseInfo::Status::Success;
        //             responseInfo.message = utility::conversions::to_utf8string(response.to_string());
        //         }
        //         else if (response.status_code() == web::http::status_codes::NotFound)
        //         {
        //             // Handle the case when the item is not found
        //             responseInfo.status = ResponseInfo::Status::NotFound;
        //             responseInfo.message = "Table item not found.";
        //         }
        //         else
        //         {
        //             // Deserialize the JSON response
        //             tableItemToken = std::make_shared<TableItemTokenMetadata>(TableItemTokenMetadata::FromJson(response.to_string()));
        //             responseInfo.status = ResponseInfo::Status::Success;
        //             responseInfo.message = utility::conversions::to_utf8string(response.to_string());
        //         }
        //     }
        //     catch (const std::exception& e)
        //     {
        //         // Handle connection errors
        //         responseInfo.status = ResponseInfo::Status::Failed;
        //         responseInfo.message = e.what();
        //     }

        //     // Callback with the results
        //     callback(tableItemToken, responseInfo); })
        //     .wait();
    }

    void RestClient::GetInfo(std::function<void(std::shared_ptr<AptosRESTModel::LedgerInfo>, AptosRESTModel::ResponseInfo)> callback)
    {
        // using namespace AptosRESTModel;
        // // Perform the HTTP request to get info
        // web::http::client::http_client client(endpoint);
        // web::uri_builder builder(U("/"));

        // client.request(web::http::methods::GET, builder.to_string())
        //     .then([=](pplx::task<web::http::http_response> task)
        //           {
        //     ResponseInfo responseInfo;
        //     std::shared_ptr<LedgerInfo> ledgerInfo = nullptr;
        //     try
        //     {
        //         web::http::http_response response = task.get();

        //         if (response.status_code() == web::http::status_codes::OK)
        //         {
        //             // Deserialize the JSON response
        //             ledgerInfo = std::make_shared<LedgerInfo>(LedgerInfo::FromJson(response.to_string()));
        //             responseInfo.status = ResponseInfo::Status::Success;
        //             responseInfo.message = utility::conversions::to_utf8string(response.to_string());
        //         }
        //         else if (response.status_code() == web::http::status_codes::NotFound)
        //         {
        //             // Handle the case when the resource is not found
        //             responseInfo.status = ResponseInfo::Status::NotFound;
        //             responseInfo.message = "Resource not found.";
        //         }
        //         else
        //         {
        //             // Deserialize the JSON response
        //             ledgerInfo = std::make_shared<LedgerInfo>(LedgerInfo::FromJson(response.to_string()));
        //             responseInfo.status = ResponseInfo::Status::Success;
        //             responseInfo.message = utility::conversions::to_utf8string(response.to_string());
        //         }
        //     }
        //     catch (const std::exception& e)
        //     {
        //         // Handle connection errors
        //         responseInfo.status = ResponseInfo::Status::Failed;
        //         responseInfo.message = e.what();
        //     }

        //     // Callback with the results
        //     callback(ledgerInfo, responseInfo); })
        //     .wait();
    }

    void RestClient::SimulateTransaction(std::function<void(std::string, AptosRESTModel::ResponseInfo)> callback,
                                         RawTransaction transaction, std::vector<uint8_t> publicKey)
    {
        // using namespace AptosRESTModel;
        // CryptoPP::SecByteBlock byteBlock(Signature::SignatureLength);
        // std::memset(byteBlock.BytePtr(), 0, byteBlock.SizeInBytes());
        // Signature emptySignature(byteBlock);
        // Authenticator authenticator{std::make_shared<Ed25519Authenticator>(PublicKey(Aptos::Utils::ByteVectorToSecBlock(publicKey)), emptySignature)};
        // SignedTransaction signedTransaction(transaction, authenticator);

        // // Perform the HTTP request to simulate a transaction
        // web::http::client::http_client client(web::uri_builder(endpoint).append(U("/transactions/simulate")).to_string());
        // web::http::http_request request(web::http::methods::POST);

        // // Set the request body with the byte array
        // request.headers().set_content_type(U("application/x.aptos.signed_transaction+bcs"));
        // request.set_body(signedTransaction.Bytes());
        // client.request(request)
        //     .then([=](pplx::task<web::http::http_response> task)
        //           {
        //     ResponseInfo responseInfo;
        //     std::string response;
        //     try
        //     {
        //         web::http::http_response httpResponse = task.get();
        //         if (httpResponse.status_code() == web::http::status_codes::OK)
        //         {
        //             response = utility::conversions::to_utf8string(httpResponse.to_string());
        //             responseInfo.status = ResponseInfo::Status::Success;
        //             responseInfo.message = response;
        //         }
        //         else if (httpResponse.status_code() == web::http::status_codes::NotFound)
        //         {
        //             responseInfo.status = ResponseInfo::Status::NotFound;
        //             responseInfo.message = "Endpoint not found.";
        //         }
        //         else
        //         {
        //             responseInfo.status = ResponseInfo::Status::Failed;
        //             responseInfo.message = utility::conversions::to_utf8string(httpResponse.to_string());
        //         }
        //     }
        //     catch (const std::exception& e)
        //     {
        //         responseInfo.status = ResponseInfo::Status::Failed;
        //         responseInfo.message = e.what();
        //     }

        //     // Callback with the results
        //     callback(response, responseInfo); })
        //     .wait();
    }

    void RestClient::SubmitBCSTransaction(std::function<void(std::string, AptosRESTModel::ResponseInfo)> callback, const SignedTransaction &signedTransaction)
    {
        // using namespace AptosRESTModel;
        // // Create an HTTP client
        // web::http::client::http_client client(endpoint.to_string() + "/transactions");
        // // Create an HTTP request with method POST
        // web::http::http_request request(web::http::methods::POST);
        // // Set the "Content-Type" header
        // request.headers().set_content_type(U("application/x.aptos.signed_transaction+bcs"));
        // // Set the request body with the signed transaction bytes
        // request.set_body(signedTransaction.Bytes());

        // // Perform the request and handle the response
        // client.request(request).then([=](web::http::http_response response)
        //                              {
        //                        ResponseInfo responseInfo;
        //                        std::string responseStr;
        //                        try {
        //                            if (response.status_code() == web::http::status_codes::OK) {
        //                                responseStr = utility::conversions::to_utf8string(response.to_string());
        //                                responseInfo.status = ResponseInfo::Status::Success;
        //                                responseInfo.message = responseStr;
        //                            } else if (response.status_code() == web::http::status_codes::NotFound) {
        //                                responseInfo.status = ResponseInfo::Status::NotFound;
        //                                responseInfo.message = "Endpoint for BCS transaction not found.";
        //                            } else {
        //                                responseInfo.status = ResponseInfo::Status::Failed;
        //                                responseInfo.message = utility::conversions::to_utf8string(response.to_string());
        //                            }
        //                        } catch (const std::exception& e) {
        //                            responseInfo.status = ResponseInfo::Status::Failed;
        //                            responseInfo.message = e.what();
        //                        }
        //                        // Callback with the results
        //                        callback(responseStr, responseInfo); })
        //     .wait();
    }

    void RestClient::View(std::function<void(std::vector<std::string>, AptosRESTModel::ResponseInfo)> callback,
                          const AptosRESTModel::ViewRequest &viewRequest)
    {
        // using namespace AptosRESTModel;
        // // Create an HTTP client
        // web::http::client::http_client client(endpoint.to_string() + U("/view"));

        // // Create an HTTP request with method POST
        // web::http::http_request request(web::http::methods::POST);

        // // Set the request body with the serialized ViewRequest
        // request.headers().set_content_type(U("application/json"));
        // request.set_body(viewRequest.ToJson().dump());

        // // Perform the request and handle the response
        // client.request(request).then([=](web::http::http_response response)
        //                              {
        //                        ResponseInfo responseInfo;
        //                        std::vector<std::string> values;
        //                        try {
        //                            if (response.status_code() == web::http::status_codes::OK) {
        //                                responseInfo.status = ResponseInfo::Status::Success;
        //                                responseInfo.message = utility::conversions::to_utf8string(response.to_string());
        //                                values = nlohmann::json::parse(response.to_string()).get<std::vector<std::string>>();
        //                            } else {
        //                                responseInfo.status = ResponseInfo::Status::Failed;
        //                                responseInfo.message = utility::conversions::to_utf8string(response.to_string());
        //                            }
        //                        } catch (const std::exception& e) {
        //                            responseInfo.status = ResponseInfo::Status::Failed;
        //                            responseInfo.message = e.what();
        //                        }

        //                        // Callback with the results
        //                        callback(values, responseInfo); })
        //     .wait();
    }

    void RestClient::SubmitTransaction(std::function<void(std::shared_ptr<AptosRESTModel::Transaction>, AptosRESTModel::ResponseInfo)> callback,
                                       Account sender, EntryFunction entryFunction)
    {
        // using namespace AptosRESTModel;
        // std::string sequenceNumber;
        // ResponseInfo responseInfo;
        // GetAccountSequenceNumber([&sequenceNumber, &responseInfo](std::string _sequenceNumber, ResponseInfo _responseInfo)
        //                          {
        // sequenceNumber = _sequenceNumber;
        // responseInfo = _responseInfo; },
        //                          sender.getAccountAddress()->ToString());

        // if (responseInfo.status != ResponseInfo::Status::Success)
        // {
        //     callback(nullptr, responseInfo);
        //     return;
        // }
        // std::time_t currentTimestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        // std::time_t expirationTimestamp = currentTimestamp + ClientConfig::EXPIRATION_TTL;

        // // Convert expiration timestamp to a string
        // std::stringstream ss;
        // ss << expirationTimestamp;
        // std::string expirationTimestampStr = ss.str();

        // TransactionRequest txnRequest;
        // txnRequest.setSender(sender.getAccountAddress()->ToString());
        // txnRequest.setSequenceNumber(sequenceNumber);
        // txnRequest.setMaxGasAmount(std::to_string(ClientConfig::MAX_GAS_AMOUNT));
        // txnRequest.setGasUnitPrice(std::to_string(ClientConfig::GAS_UNIT_PRICE));
        // txnRequest.setExpirationTimestampSecs(expirationTimestampStr);
        // auto txnRequestJson = txnRequest.ToJson();
        // std::string encodedSubmission;
        // EncodeSubmission([&encodedSubmission](const std::string &_encodedSubmission)
        //                  { encodedSubmission = _encodedSubmission; },
        //                  txnRequestJson.dump());
        // std::vector<uint8_t> toSign = Aptos::Utils::ByteArrayFromHexString(Aptos::Utils::trim(encodedSubmission, "\"").substr(2));

        // Signature signature = sender.Sign(Aptos::Utils::ByteVectorToSecBlock(toSign));

        // SignatureData sigData;
        // sigData.setType(Constants::ED25519_SIGNATURE);
        // sigData.setPublicKey(sender.getPublicKey()->ToString());
        // sigData.setSignature(signature.ToString());
        // txnRequest.setSignature(sigData);
        // txnRequestJson = txnRequest.ToJson();

        // std::string transactionURL = endpoint.to_string() + "/transactions";
        // web::uri_builder builder(transactionURL);
        // web::http::client::http_client client(builder.to_uri());

        // web::http::http_request request(web::http::methods::POST);
        // request.headers().set_content_type("application/json");
        // request.set_body(txnRequestJson.dump());

        // client.request(request).then([&](web::http::http_response response)
        //                              {
        //                        if (response.status_code() == web::http::status_codes::OK || response.status_code() == web::http::status_codes::Accepted)
        //                        {
        //                            return response.extract_string();
        //                        }
        //                        else
        //                        {
        //                            responseInfo.status = ResponseInfo::Status::Failed;
        //                            responseInfo.message = "Error while submitting transaction. " + response.to_string();
        //                            callback(nullptr, responseInfo);
        //                            return pplx::task_from_result(std::string());
        //                        } })
        //     .then([&](pplx::task<std::string> previousTask)
        //           {
        //     try
        //     {
        //         std::string response = previousTask.get();
        //         auto transaction = std::make_shared<Transaction>(Transaction::FromJson(response));

        //         responseInfo.status = ResponseInfo::Status::Success;
        //         responseInfo.message = response;
        //         callback(transaction, responseInfo);
        //     }
        //     catch (const std::exception& e)
        //     {
        //         std::cerr << "Exception: " << e.what() << std::endl;
        //         responseInfo.status = ResponseInfo::Status::Failed;
        //         responseInfo.message = "Error in processing the response.";
        //         callback(nullptr, responseInfo);
        //     } })
        //     .wait();
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
                try
                {
                    auto transaction = Transaction::FromJson(responseInfoTxnPending.message);
                    if (transaction.getSuccess())
                    {
                        responseInfo.status = ResponseInfo::Status::Success;
                        responseInfo.message = responseInfoTxnPending.message;

                        isTxnSuccessful = true;
                        break;
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Exception: " << e.what() << std::endl;
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
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));
        callback(isTxnSuccessful, responseInfo);
    }

    void RestClient::TransactionPending(std::function<void(bool, AptosRESTModel::ResponseInfo)> callback, const std::string &txnHash)
    {
        // using namespace AptosRESTModel;
        // web::uri_builder builder(endpoint.to_string() + U("/transactions/by_hash/") + utility::conversions::to_string_t(txnHash));
        // web::http::client::http_client client(builder.to_uri());

        // try
        // {
        //     web::http::http_response response = client.request(web::http::methods::GET).get();

        //     ResponseInfo responseInfo;

        //     if (response.status_code() == web::http::status_codes::OK)
        //     {
        //         auto transactionResult = Transaction::FromJson(response.to_string());
        //         bool isPending = transactionResult.getType() == "pending_transaction";

        //         responseInfo.status = ResponseInfo::Status::Success;
        //         responseInfo.message = utility::conversions::to_utf8string(response.to_string());

        //         callback(isPending, responseInfo);
        //     }
        //     else if (response.status_code() == web::http::status_codes::NotFound)
        //     {
        //         responseInfo.status = ResponseInfo::Status::Failed;
        //         responseInfo.message = "Transaction Not Found: " + std::to_string(response.status_code());

        //         callback(true, responseInfo);
        //     }
        //     else
        //     {
        //         responseInfo.status = ResponseInfo::Status::Failed;
        //         responseInfo.message = "Transaction Call Error: " + std::to_string(response.status_code()) + " : " +
        //                                utility::conversions::to_utf8string(response.to_string());

        //         callback(true, responseInfo);
        //     }
        // }
        // catch (const std::exception &e)
        // {
        //     std::cerr << "Exception: " << e.what() << std::endl;
        // }
    }

    void RestClient::TransactionByHash(std::function<void(AptosRESTModel::Transaction, AptosRESTModel::ResponseInfo)> callback, const std::string &txnHash)
    {
        // using namespace AptosRESTModel;
        // web::uri_builder builder(endpoint.to_string() + U("/transactions/by_hash/") + utility::conversions::to_string_t(txnHash));
        // web::http::client::http_client client(builder.to_uri());

        // try
        // {
        //     web::http::http_response response = client.request(web::http::methods::GET).get();

        //     ResponseInfo responseInfo;

        //     if (response.status_code() == web::http::status_codes::OK)
        //     {
        //         auto transactionResult = Transaction::FromJson(response.to_string());
        //         responseInfo.status = ResponseInfo::Status::Success;
        //         responseInfo.message = utility::conversions::to_utf8string(response.to_string());
        //         callback(transactionResult, responseInfo);
        //     }
        //     else if (response.status_code() == web::http::status_codes::NotFound)
        //     {
        //         responseInfo.status = ResponseInfo::Status::Failed;
        //         responseInfo.message = "Transaction Not Found: " + std::to_string(response.status_code());

        //         callback({}, responseInfo);
        //     }
        //     else
        //     {
        //         responseInfo.status = ResponseInfo::Status::Failed;
        //         responseInfo.message = "Transaction Call Error: " + std::to_string(response.status_code()) + " : " +
        //                                utility::conversions::to_utf8string(response.to_string());

        //         callback({}, responseInfo);
        //     }
        // }
        // catch (const std::exception &e)
        // {
        //     std::cerr << "Exception: " << e.what() << std::endl;
        // }
    }

    void RestClient::CreateBCSSignedTransaction(std::function<void(std::shared_ptr<SignedTransaction>)> Callback, Account Sender, TransactionPayload Payload)
    {
        std::shared_ptr<RawTransaction> rawTransaction;
        CreateBCSTransaction([&rawTransaction](std::shared_ptr<RawTransaction> _rawTransaction)
                             { rawTransaction = _rawTransaction; },
                             Sender, Payload);

        Signature signature = Sender.Sign(Aptos::Utils::ByteVectorToSecBlock(rawTransaction->Keyed()));
        Authenticator authenticator(std::make_shared<Ed25519Authenticator>(*Sender.getPublicKey(), signature));

        Callback(std::make_shared<SignedTransaction>(*rawTransaction, authenticator));
    }

    void RestClient::CreateBCSTransaction(std::function<void(std::shared_ptr<RawTransaction>)> Callback, Account Sender, TransactionPayload payload)
    {
        using namespace AptosRESTModel;
        std::string sequenceNumber = "";
        ResponseInfo responseInfo;

        //    GetAccountSequenceNumber(sequenceNumber, responseInfo, Sender.AccountAddress);

        //    if (responseInfo.status != ResponseInfo::Status::Success) {
        //        throw std::runtime_error("Unable to get sequence number for: " + Sender.AccountAddress + ".\n" + responseInfo.message);
        //    }

        //    if (ChainId == nullptr) {
        //        SetUpRestClient();
        //    }

        //    ulong expirationTimestamp = static_cast<ulong>(DateTime::ToUnixTimestamp() + Constants::EXPIRATION_TTL);

        //    RawTransaction rawTxn(
        //        Sender.AccountAddress,
        //        std::stoi(sequenceNumber),
        //        payload,
        //        ClientConfig::MAX_GAS_AMOUNT,
        //        ClientConfig::GAS_UNIT_PRICE,
        //        expirationTimestamp,
        //        static_cast<int>(ChainId)
        //        );

        //    Callback(rawTxn);
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
            TagSequence({}),
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
            Transaction transaction = Transaction::FromJson(responseInfo.message);
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
            TagSequence({}),
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
        // web::http::client::http_client client(endpoint.to_string() + "/transactions/encode_submission");
        // web::http::http_request request(web::http::methods::POST);
        // request.headers().set_content_type(U("application/json"));
        // request.set_body(txnRequestJson);
        // client.request(request).then([=](web::http::http_response response)
        //                              {
        //                        if (response.status_code() == web::http::status_codes::OK) {
        //                            callback(response.to_string());
        //                        } })
        //     .wait();
    }

    void RestClient::EncodeSubmissionAsBytes(const std::function<void(const std::vector<uint8_t> &)> &callback, const std::string &txnRequestJson)
    {
        // web::http::client::http_client client(endpoint.to_string() + "/transactions/encode_submission");
        // web::http::http_request request(web::http::methods::POST);
        // request.headers().set_content_type(U("application/json"));
        // request.set_body(txnRequestJson);
        // client.request(request).then([=](web::http::http_response response)
        //                              {
        //                        if (response.status_code() == web::http::status_codes::OK) {
        //                            std::string msg = response.to_string();
        //                            std::vector<uint8_t> byteVector(msg.begin(), msg.end());
        //                            callback(byteVector);
        //                        } })
        //     .wait();
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
            TagSequence({}),
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
        createCollectionTxn = AptosRESTModel::Transaction::FromJson(submitBcsTxnJsonResponse);
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
            TagSequence({}),
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
        createTokenTxn = AptosRESTModel::Transaction::FromJson(submitBcsTxnJsonResponse);
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
            TagSequence({}),
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
        offerTokenTxn = AptosRESTModel::Transaction::FromJson(submitBcsTxnJsonResponse);
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
            TagSequence({}),
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
        claimTokenTxn = AptosRESTModel::Transaction::FromJson(submitBcsTxnJsonResponse);
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
            AptosRESTModel::AccountResourceTokenStore::FromJson(tokenStoreResourceResp);
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

    void RestClient::GetTokenData(std::function<void(AptosRESTModel::TableItemTokenMetadata, AptosRESTModel::ResponseInfo)> callback, AccountAddress creator, std::string collectionName, std::string tokenName, int propertyVersion)
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
            callback(AptosRESTModel::TableItemTokenMetadata(), responseInfo);
        }

        AptosRESTModel::ResourceCollection resourceCollection = AptosRESTModel::ResourceCollection::FromJson(collectionResourceResp);
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

        callback(*tableItemTokenMetadata, responseInfo);
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

        AptosRESTModel::ResourceCollection resourceCollection = AptosRESTModel::ResourceCollection::FromJson(collectionResourceResp);
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
            TagSequence({}),
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
        // utility::string_t accountsURL = endpoint.to_string() + "/accounts/" + accountAddress.ToString() + "/resource/" + resourceType;
        // web::http::client::http_client client(accountsURL);
        // web::http::http_request request(web::http::methods::GET);
        // client.request(request).then([=](web::http::http_response response)
        //                              {
        //                        if (response.status_code() == web::http::status_codes::OK) {
        //                            callback(true, response.status_code(), response.to_string());
        //                        } else {
        //                            callback(false, response.status_code(), response.to_string());
        //                        } })
        //     .wait();
    }
}
