#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include <memory>
#include "Model/AccountData.h"
#include "Model/LedgerInfo.h"
#include "Model/ResourceCollection.h"
#include "Model/ResponseInfo.h"
#include "Model/AccountResourceCoin.h"
#include "Model/TableItemToken/TableItemToken.h"
#include "Model/TableItemToken/TableItemTokenMetadata.h"
#include "Model/TokenIdRequest.h"
#include "../Transactions/Rawtransaction.h"
#include "../Transactions/SignedTransaction.h"
#include "Model/Transaction.h"
#include "Model/ViewRequest.h"
#include "../Accounts/Account.h"
#include "../BCS/EntryFunctions/EntryFunction.h"

namespace Aptos::Rest
{
    class APTOS_API ClientConfig
    {
    public:
        static constexpr int EXPIRATION_TTL = 600;
        static constexpr int GAS_UNIT_PRICE = 100;
        static constexpr int MAX_GAS_AMOUNT = 100000;
        static constexpr int TRANSACTION_WAIT_IN_SECONDS = 20; // Amount of seconds to wait during each polling cycle.
    };

    class APTOS_API RestClient
    {
    public:
        void SetEndpoint(const std::string &url);
        explicit RestClient(const std::string &url);
        RestClient() = default;
        void SetUp();
        /// <summary>
        /// Get Account Details.
        /// Return the authentication key and the sequence number for an account address. Optionally, a ledger version can be specified.
        /// If the ledger version is not specified in the request, the latest ledger version is used.
        /// </summary>
        /// <param name="callback">Callback function used after response is received.</param>
        /// <param name="accountAddress">Address of the account.</param>
        /// <returns>Calls <c>callback</c> function with <c>(AccountData, ResponseInfo)</c>: \n
        /// An object that contains the account's:
        /// <c>sequence_number</c>, a string containing a 64-bit unsigned integer.
        /// Example: <code>32425224034</code>
        /// <c>authentication_key</c> All bytes (Vec) data is represented as hex-encoded string prefixed with 0x and fulfilled with two hex digits per byte.
        /// Unlike the Address type, HexEncodedBytes will not trim any zeros.
        /// Example: <code>0x88fbd33f54e1126269769780feb24480428179f552e2313fbe571b72e62a1ca1</code>, it is null if the request fails \n
        /// and a response object that contains the response details.
        /// </returns>
        void GetAccount(std::function<void(std::shared_ptr<AptosRESTModel::AccountData>, AptosRESTModel::ResponseInfo)> callback,
                        const Accounts::AccountAddress &accountAddress);
        /// <summary>
        /// Get an account's balance.
        ///
        /// The <c>/account</{address}/resource/{coin_type}</c> endpoint for AptosCoin returns the following response:
        /// Gets Account Sequence Number
        /// </summary>
        /// <param name="callback">Callback function used after response is received.</param>
        /// <param name="accountAddress">Address of the account.</param>
        /// <returns>Calls <c>callback</c> function with <c>(string, ResponseInfo)</c>: \n
        /// A Sequence number as a string - null if the request fails, and a response object containing the response details. </returns>
        void GetAccountSequenceNumber(std::function<void(std::string, AptosRESTModel::ResponseInfo)> callback, const Accounts::AccountAddress &accountAddress);

        /// <summary>
        /// Get an account's balance.
        ///
        /// The <c>/account</{address}/resource/{coin_type}</c> endpoint for AptosCoin returns the following response:
        /// <code>
        /// {
        ///     "type":"0x1::coin::CoinStore<0x1::aptos_coin::AptosCoin>",
        ///     "data":{
        ///         "coin":{
        ///             "value":"3400034000"
        ///         },
        ///         "deposit_events":{
        ///             "counter":"68",
        ///             "guid":{
        ///                 "id":{
        ///                     "addr":"0xd89fd73ef7c058ccf79fe4c1c38507d580354206a36ae03eea01ddfd3afeef07",
        ///                     "creation_num":"2"
        ///                 }
        ///             }
        ///         },
        ///         "frozen":false,
        ///         "withdraw_events":{
        ///             "counter":"0",
        ///             "guid":{
        ///                 "id":{
        ///                     "addr":"0xd89fd73ef7c058ccf79fe4c1c38507d580354206a36ae03eea01ddfd3afeef07",
        ///                     "creation_num":"3"
        ///                 }
        ///             }
        ///         }
        ///     }
        /// }
        /// </code>
        /// </summary>
        /// <param name="callback">Callback function used after response is received.</param>
        /// <param name="accountAddress">Address of the account.</param>
        /// <returns>Calls <c>callback</c> function with <c>(AccountResourceCoin.Coin, ResponseInfo)</c>: \n
        /// A representation of the coin, and an object containing the response details.</returns>
        void GetAccountBalance(std::function<void(AptosRESTModel::AccountResourceCoin::Coin, AptosRESTModel::ResponseInfo)> callback,
                               const Accounts::AccountAddress &accountAddress);
        /// <summary>
        /// Get a resource of a given type from an account.
        /// NOTE: The response is a complex object of types only known to the developer writing the contracts.
        /// This function return a string and expect the developer to deserialize it into an object.
        /// See <see cref="GetAccountResourceCollection(Action{ResourceCollection, ResponseInfo}, AccountAddress, string)">GetAccountResourceCollection</see> for an example.
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="accountAddress">Address of the account.</param>
        /// <param name="resourceType">Type of resource being queried for.</param>
        /// <returns>Calls <c>callback</c> function with <c>(bool, long, string)</c>: \n
        /// -- bool: success boolean \n
        /// -- long: - error code, string - JSON response to be deserialized by the consumer of the function\n
        /// -- string: - the response which may contain the resource details</returns>
        void GetAccountResource(std::function<void(bool, long, std::string)> callback,
                                const Accounts::AccountAddress &accountAddress,
                                const std::string &resourceType = "",
                                const std::string &ledgerVersion = "");
        /// <summary>
        /// Get resources (in a JSON format) from a given account.
        /// </summary>
        /// <param name="callback">allback function used when response is received.</param>
        /// <param name="accountAddress">Address of the account.</param>
        /// <param name="ledgerVersion">Type of resource being queried for.</param>
        /// <returns></returns>
        void GetAccountResources(std::function<void(bool, long, std::string)> callback,
                                 const Accounts::AccountAddress &accountAddress,
                                 const std::string &ledgerVersion = "");
        /// <summary>
        /// Get Account Resource
        /// </summary>
        /// <param name="callback">Callback function used after response is received.</param>
        /// <param name="accountAddress">Address of the account.</param>
        /// <param name="resourceType">Type of resource being queried for.</param>
        /// <returns>Calls <c>callback</c> function with <c>(ResourceCollection, ResponseInfo)</c>:\n
        /// An object representing a collection resource - null if the request fails, and a response object contains the response details.</returns>
        void GetAccountResourceCollection(std::function<void(std::shared_ptr<AptosRESTModel::ResourceCollection>, AptosRESTModel::ResponseInfo)> callback,
                                          const Accounts::AccountAddress &accountAddress,
                                          const std::string &resourceType);
        /// <summary>
        /// Gets table item that represents a coin resource
        /// See <see cref="GetTableItem(Action{string}, string, string, string, string)">GetTableItem</see>
        /// </summary>
        /// <param name="callback">Callback function used after response is received.</param>
        /// <param name="handle">The identifier for the given table.</param>
        /// <param name="keyType">String representation of an on-chain Move tag that is exposed in the transaction.</param>
        /// <param name="valueType">String representation of an on-chain Move type value.</param>
        /// <param name="key">The value of the table item's key, e.g. the name of a collection</param>
        /// <returns>Calls <c>callback</c> function with <c>(AccountResourceCoing, ResponseInfo)</c>:\n
        /// An object representing the account resource that holds the coin's information - null if the request fails, and a response object the contains the response details.</returns>
        void GetTableItemCoin(std::function<void(std::shared_ptr<AptosRESTModel::AccountResourceCoin>, AptosRESTModel::ResponseInfo)> callback,
                              const std::string &handle,
                              const std::string &keyType,
                              const std::string &valueType,
                              const std::string &key);
        /// <summary>
        /// Get a  table item at a specific ledger version from the table identified
        /// by the handle {table_handle} in the path and a [simple] "key" (TableItemRequest)
        /// provided by the request body.
        ///
        /// Further details are provider <see cref="https://fullnode.devnet.aptoslabs.com/v1/spec#/operations/get_table_item">here</see>
        /// </summary>
        /// <param name="callback">Callback function used after response is received.</param>
        /// <param name="handle">The identifier for the given table</param>
        /// <param name="keyType">String representation of an on-chain Move tag that is exposed in the transaction, e.g. "0x1::string::String"</param>
        /// <param name="valueType">String representation of an on-chain Move type value, e.g. "0x3::token::CollectionData"</param>
        /// <param name="key">The value of the table item's key, e.g. the name of a collection.</param>
        /// <returns>Calls <c>callback</c> function with a JSON object representing a table item - null if the request fails.</returns>
        void GetTableItem(std::function<void(std::string)> callback,
                          const std::string &handle,
                          const std::string &keyType,
                          const std::string &valueType,
                          const std::string &key);
        /// <summary>
        /// Get a  table item for a NFT from the table identified
        /// by the handle {table_handle} in the path and a complex key provided by the request body.
        ///
        /// See <see cref="GetTableItem(Action{string}, string, string, string, string)">GetTableItem</see> for a get table item using a generic string key.
        /// </summary>
        /// <param name="callback">Callback function used after response is received.</param>
        /// <param name="handle">The identifier for the given table.</param>
        /// <param name="keyType">String representation of an on-chain Move tag that is exposed in the transaction.</param>
        /// <param name="valueType">String representation of an on-chain Move type value.</param>
        /// <param name="key">A complex key object used to search for the table item. For example:
        /// <code>
        /// {
        ///     "token_data_id":{
        ///         "creator":"0xcd7820caacab04fbf1d7e563f4d329f06d2ce3140d654729d99258b5b68a27bf",
        ///         "collection":"Alice's",
        ///         "name":"Alice's first token"
        ///     },
        ///     "property_version":"0"
        /// }
        /// </code>
        /// </param>
        /// <returns>Calls <c>callback</c> function with <c>(TableItemToken, ResponseInfo)</c>: \n
        /// An object containing the details of the token - null if the request fails, and a response object containing the response details.
        /// </returns>
        void GetTableItemNFT(std::function<void(std::shared_ptr<AptosRESTModel::TableItemToken>, AptosRESTModel::ResponseInfo)> callback,
                             const std::string &handle,
                             const std::string &keyType,
                             const std::string &valueType,
                             AptosRESTModel::TokenIdRequest key);
        /// <summary>
        /// Get a table item that contains a token's (NFT) metadata.
        /// In this case we are using a complex key to retrieve the table item.
        ///
        /// Note: the response received from the REST <c>/table</c>  for this methods
        /// has a particular format specific to the SDK example.
        ///
        /// Developers will have to implement their own custom object's to match
        /// the properties of the NFT, meaning the content in the table item.
        ///
        /// The response for <c>/table</c> in the SDK example has the following format:
        /// <code>
        /// {
        ///     "default_properties":{
        ///         "map":{
        ///             "data":[ ]
        ///         }
        ///     },
        ///     "description":"Alice's simple token",
        ///     "largest_property_version":"0",
        ///     "maximum":"1",
        ///     "mutability_config":{
        ///         "description":false,
        ///         "maximum":false,
        ///         "properties":false,
        ///         "royalty":false,
        ///         "uri":false
        ///     },
        ///     "name":"Alice's first token",
        ///     "royalty":{
        ///         "payee_address":"0x8b8a8935cd90a87eaf47c7f309b6935e305e48b47f95982d65153b03032b3f33",
        ///         "royalty_points_denominator":"1000000",
        ///         "royalty_points_numerator":"0"
        ///     },
        ///     "supply":"1",
        ///     "uri":"https://aptos.dev/img/nyan.jpeg"
        /// }
        /// </code>
        /// </summary>
        /// <param name="callback">Callback function used after response is received.</param>
        /// <param name="handle">The identifier for the given table.</param>
        /// <param name="keyType">String representation of an on-chain Move tag that is exposed in the transaction.</param>
        /// <param name="valueType">String representation of an on-chain Move type value.</param>
        /// <param name="key">A complex key object used to search for the table item. In this case it's a TokenDataId object that contains the token / collection info</param>
        /// <returns>Calls <c>callback</c> function with <c>(TableItemTokenMetadata, ResponseInfo)</c>: \n
        /// An object that represent the NFT's metadata - null if the request fails, and a response object with the response details.
        /// </returns>
        void GetTableItemTokenData(std::function<void(std::shared_ptr<AptosRESTModel::TableItemTokenMetadata>, AptosRESTModel::ResponseInfo)> callback,
                                   const std::string &handle,
                                   const std::string &keyType,
                                   const std::string &valueType,
                                   AptosRESTModel::TokenDataId key);
        /// <summary>
        /// Get the latest ledger information, including data such as chain ID, role type, ledger versions, epoch, etc.
        /// </summary>
        /// <param name="callback">Callback function used after response is received.</param>
        /// <returns>Calls <c>callback</c>function with <c>(LedgerInfo, response)</c>: \n
        /// An object that contains the chain details - null if the request fails, and a response object that contains the response details. </returns>
        void GetInfo(std::function<void(std::shared_ptr<AptosRESTModel::LedgerInfo>, AptosRESTModel::ResponseInfo)> callback);

        void SimulateTransaction(std::function<void(std::string, AptosRESTModel::ResponseInfo)> callback,
                                 BCS::RawTransaction transaction, std::vector<uint8_t> publicKey);
        /// <summary>
        /// Submits a BCS transaction.
        /// </summary>
        /// <param name="callback">Callback function used after response is received with the JSON response.</param>
        /// <param name="SignedTransaction">The signed transaction.</param>
        /// <returns></returns>
        void SubmitBCSTransaction(std::function<void(std::string, AptosRESTModel::ResponseInfo)> callback, const BCS::SignedTransaction &signedTransaction);
        /// <summary>
        /// Execute the Move view function with the given parameters and return its execution result.
        ///
        /// Even if the function returns a single value, it will be wrapped in an array.
        ///
        /// Usage example where we determine an accounts AptosCoin balance:
        /// <code>
        /// string[] data = new string[] {};
        /// ViewRequest viewRequest = new ViewRequest();
        /// viewRequest.Function = "0x1::coin::balance";
        /// viewRequest.TypeArguments = new string[] { "0x1::aptos_coin::AptosCoin" };
        /// viewRequest.Arguments = new string[] { bobAddress.ToString() };
        /// Coroutine getBobAccountBalanceView = StartCoroutine(RestClient.Instance.View((_data, _responseInfo) =>
        /// {
        ///     data = _data;
        ///     responseInfo = _responseInfo;
        /// }, viewRequest));
        /// yield return getBobAccountBalanceView;
        /// if (responseInfo.status == ResponseInfo.Status.Failed) {
        ///     Debug.LogError(responseInfo.message);
        ///     yield break;
        /// }
        /// Debug.Log("Bob's Balance After Funding: " + ulong.Parse(data[0]));
        /// </code>
        /// </summary>
        /// <param name="callback">Callback function used after response is received.</param>
        /// <param name="viewRequest">The payload for the view function</param>
        /// <returns>A vec containing the values returned from the view functions.</returns>
        void View(std::function<void(std::vector<std::string>, AptosRESTModel::ResponseInfo)> callback, const AptosRESTModel::ViewRequest &viewRequest);
        /// <summary>
        /// 1) Generates a transaction request \n
        /// 2) submits that to produce a raw transaction \n
        /// 3) signs the raw transaction \n
        /// 4) submits the signed transaction \n
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="sender">Account submitting the transaction.</param>
        /// <param name="payload">Transaction payload.</param>
        /// <returns>Calls <c>callback</c>function with <c>(Transaction, ResponseInfo)</c>:\n
        /// An object that represents the transaction submitted - null if the transaction fails, and a response object with the response detials.
        /// </returns>
        void SubmitTransaction(std::function<void(std::shared_ptr<AptosRESTModel::Transaction>, AptosRESTModel::ResponseInfo)> callback,
                               Accounts::Account sender,
                               BCS::EntryFunction entryFunction);
        /// <summary>
        /// A Coroutine that polls for a transaction hash until it is confimred in the blockchain
        /// Times out if the transaction hash is not found after querying for N times.
        ///
        /// Waits for a transaction query to respond whether a transaction submitted has been confirmed in the blockchain.
        /// Queries for a given transaction hash (txnHash) using <see cref="TransactionPending"/>
        /// by polling / looping until we find a "Success" transaction response, or until it times out after <see cref="TransactionWaitInSeconds"/>.
        ///
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="txnHash">Transaction hash.</param>
        /// <returns>Calls <c>callback</c> function with (bool, ResponseInfo): \n
        /// A boolean that is: \n
        /// -- true if the transaction hash was found after polling and the transaction was succesfully commited in the blockhain \n
        /// -- false if we did not find the transaction hash and timed out \n
        ///
        /// A response object that contains the response details.
        /// </returns>
        void WaitForTransaction(std::function<void(bool, AptosRESTModel::ResponseInfo)> callback, const std::string &txnHash);
        /// <summary>
        /// Query to see if transaction has been 'confirmed' in the blockchain by using the transaction hash.
        /// A 404 error will be returned if the transaction hasn't been confirmed.
        /// Once the transaction is confirmed it will have a `pending_transaction` state.
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="txnHash">Transaction hash being queried for.</param>
        /// <returns>Calls <c>callback</c>function with <c>(bool, ResponseInfo)</c>: \n
        /// A boolean that is: \n
        /// -- true if transaction is still pending / hasn't been found, meaning 404, error in response, or `pending_transaction` is true \n
        /// -- false if transaction has been found, meaning `pending_transaction` is true \n
        ///
        /// A response object that contains the response details.
        /// </returns>
        void TransactionPending(std::function<void(bool, AptosRESTModel::ResponseInfo)> callback, const std::string &txnHash);
        void TransactionByHash(std::function<void(AptosRESTModel::Transaction, AptosRESTModel::ResponseInfo)> callback, const std::string &txnHash);
        void CreateBCSSignedTransaction(std::function<void(std::shared_ptr<BCS::SignedTransaction>)> Callback, Accounts::Account Sender, BCS::TransactionPayload Payload);
        void CreateBCSTransaction(std::function<void(std::shared_ptr<BCS::RawTransaction>)> Callback, Accounts::Account Sender, BCS::TransactionPayload payload);
        /// <summary>
        /// Transfer a given coin amount from a given Account to the recipient's account Address.
        /// Returns the sequence number of the transaction used to transfer.
        ///
        /// NOTE: Recipient address must hold APT before hand, and or have been airdrop APT if testing on devnet.
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="sender">Account executing the transfer.</param>
        /// <param name="to">Address of recipient.</param>
        /// <param name="amount">Amount of tokens.</param>
        /// <returns>Calls <c>callback</c>function with <c>(Transaction, ResponseInfo)</c>: \n
        /// An object the represents the transaction submitted - null if the transfer failed, and a response object that contains the response details.
        /// </returns>
        void Transfer(std::function<void(std::shared_ptr<AptosRESTModel::Transaction>, AptosRESTModel::ResponseInfo)> callback, Accounts::Account sender, const std::string &to, long amount);
        void BCSTransfer(std::function<void(std::string, AptosRESTModel::ResponseInfo)> Callback, Accounts::Account Sender, Accounts::AccountAddress Recipient, int Amount);
        /// <summary>
        /// Encodes submission.
        /// This endpoint accepts an EncodeSubmissionRequest, which internally is a UserTransactionRequestInner
        /// (and optionally secondary signers) encoded as JSON, validates the request format, and then returns that request encoded in BCS.
        /// The client can then use this to create a transaction signature to be used in a SubmitTransactionRequest, which it then passes to the /transactions POST endpoint.
        ///
        /// If you are using an SDK that has BCS support, such as the official Rust, TypeScript, or Python SDKs, you may use the BCS encoding instead of this endpoint.
        ///
        /// To sign a message using the response from this endpoint:
        /// - Decode the hex encoded string in the response to bytes.
        /// - Sign the bytes to create the signature.
        /// - Use that as the signature field in something like Ed25519Signature, which you then use to build a TransactionSignature.
        /// </summary>
        /// <param name="callback">Callback function used after response is received.</param>
        /// <param name="txnRequestJson">Transaction request in JSON format.</param>
        /// <returns>
        /// Calls <c>callback</c> function with: \n
        /// All bytes (Vec) data that is represented as hex-encoded string prefixed with 0x and fulfilled with two hex digits per byte.
        /// Unlike the Address type, HexEncodedBytes will not trim any zeros.
        /// Example: <code>0x88fbd33f54e1126269769780feb24480428179f552e2313fbe571b72e62a1ca1</code>
        /// </returns>
        void EncodeSubmission(const std::function<void(const std::string &)> &callback, const std::string &txnRequestJson);
        /// <summary>
        /// Encodes submission. See <see cref="EncodeSubmission(Action{string}, string)">EncodeSubmission</see>.
        /// </summary>
        /// <param name="callback">Callback function used after response is received.</param>
        /// <param name="txnRequestJson">Transaction request in JSON format.</param>
        /// <returns>Calls <c>callback</c>function with a byte array representing the encoded submission.</returns>
        void EncodeSubmissionAsBytes(const std::function<void(const std::vector<uint8_t> &)> &callback, const std::string &txnRequestJson);
        /// <summary>
        /// Create a NFT collection
        /// </summary>
        /// <param name="callback">Callback function used after response is received.</param>
        /// <param name="sender">Creator of the collection.</param>
        /// <param name="collectionName">Name of collection.</param>
        /// <param name="collectionDescription">Description of the collection.</param>
        /// <param name="uri">Collection's URI</param>
        /// <returns>Calls <c>callback</c> function with <c>(Transaction, ResponseInfo)</c>: \n
        /// An object the represents the transaction submitted - null if the transaction to create a collection failed, and a response object that contains the response details.
        /// </returns>
        void CreateCollection(
            std::function<void(AptosRESTModel::Transaction, AptosRESTModel::ResponseInfo)> callback,
            Accounts::Account sender,
            std::string collectionName,
            std::string collectionDescription,
            std::string uri);
        /// <summary>
        /// Create Non-Fungible Token (NFT)
        /// See token <see cref="https://github.com/aptos-labs/aptos-core/blob/main/aptos-move/framework/aptos-token/sources/token.move#L365">reference.</see>
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="senderRoyaltyPayeeAddress">Creator of the token, also the account the that will receive royalties. </param>
        /// <param name="collectionName">Name of the collection to which the token belongs to.</param>
        /// <param name="tokenName">Name of the token</param>
        /// <param name="description">Description of the token being minted</param>
        /// <param name="supply">Token supply</param>
        /// <param name="max">Max number of mints</param>
        /// <param name="uri">URI of where the token's asset lives (e.g. JPEG)</param>
        /// <param name="royaltyPointsPerMillion">Royalties defined in the millionths</param>
        /// <returns>Calls <c>callback</c> function with <c>(Transaction, ResponseInfo)</c>: \n
        /// An object the represents the transaction submitted - null if the transaction to create a token failed, and a response object that contains the response details.
        /// </returns>
        void CreateToken(
            std::function<void(AptosRESTModel::Transaction, AptosRESTModel::ResponseInfo)> callback,
            Accounts::Account senderRoyaltyPayeeAddress,
            std::string collectionName,
            std::string tokenName,
            std::string description,
            int supply,
            int max,
            std::string uri,
            int royaltyPointsPerMillion);
        /// <summary>
        /// Offer a token to a given address.
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="account">Accounts::Account offering the token.</param>
        /// <param name="receiver">Address of recipient.</param>
        /// <param name="creator">Address of token creator.</param>
        /// <param name="collectionName">Name of the collection.</param>
        /// <param name="tokenName">Name of the token.</param>
        /// <param name="amount">Amount being offered.</param>
        /// <param name="propertyVersion">ersion of the token.</param>
        /// <returns>Calls <c>callback</c> function with <c>(Transaction, ResponseInfo)</c>: \n
        /// An object the represents the transaction submitted - null if the transaction to offer a token failed, and a response object that contains the response details.
        /// </returns>
        void OfferToken(
            std::function<void(std::shared_ptr<AptosRESTModel::Transaction>, AptosRESTModel::ResponseInfo)> callback,
            Accounts::Account account,
            Accounts::AccountAddress receiver,
            Accounts::AccountAddress creator,
            std::string collectionName,
            std::string tokenName,
            int amount,
            int propertyVersion  = 0);
        /// <summary>
        /// Claim a token that was offered by <paramref name="sender"/>
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="account">Accounts::Account making the claim</param>
        /// <param name="sender">Address of the sender of the non-fungible token (NFT)</param>
        /// <param name="creator">Address of the creator of the token (NFT)</param>
        /// <param name="collectionName">Name of the NFT collection</param>
        /// <param name="tokenName">Name of the token</param>
        /// <param name="propertyVersion">Token version, defaults to 0</param>
        /// <returns>Calls <c>callback</c>function with <c>(Transaction, ResponseInfo)</c>: \n
        /// An object the represents the transaction submitted - null if the transaction to claim a token failed, and a response object that contains the response details.
        /// </returns>
        void ClaimToken(std::function<void(std::shared_ptr<AptosRESTModel::Transaction>, AptosRESTModel::ResponseInfo)> callback,
                        Accounts::Account account,
                        Accounts::AccountAddress sender,
                        Accounts::AccountAddress creator,
                        std::string collectionName,
                        std::string tokenName,
                        int propertyVersion);
        /// <summary>
        /// Get token information.
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="ownerAddress">Address of token owner.</param>
        /// <param name="creatorAddress">Address of token creator.</param>
        /// <param name="collectionName">Name of the collection.</param>
        /// <param name="tokenName">Name of the token.</param>
        /// <param name="propertyVersion">Version of the token.</param>
        /// <returns>Calls <c>callback</c> function with <c>(TableItemToken, ResponseInfo)</c>: \n
        /// An object the represents the transaction submitted - null if the transaction to get a token failed, and a response object that contains the response details.
        /// </returns>
        void GetToken(
            std::function<void(AptosRESTModel::TableItemToken, AptosRESTModel::ResponseInfo)> callback,
            Accounts::AccountAddress ownerAddress,
            Accounts::AccountAddress creatorAddress,
            std::string collectionName,
            std::string tokenName,
            int propertyVersion);
        /// <summary>
        /// Get balance for a given non-fungible token (NFT).
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="ownerAddress">Address of token owner.</param>
        /// <param name="creatorAddress">Address of token creator.</param>
        /// <param name="collectionName">Name of the collection.</param>
        /// <param name="tokenName">Name of the token.</param>
        /// <param name="propertyVersion">Version of the token.</param>
        /// <returns>Token balance as a string.</returns>
        void GetTokenBalance(
            std::function<void(std::string)> callback,
            Accounts::AccountAddress ownerAddress,
            Accounts::AccountAddress creatorAddress,
            std::string collectionName,
            std::string tokenName,
            int propertyVersion);
        /// <summary>
        /// Read Collection's token data table.
        /// An example
        /// <code>
        /// {
        ///     "default_properties":{
        ///         "map":{
        ///             "data":[ ]
        ///         }
        ///     },
        ///     "description":"Alice's simple token",
        ///     "largest_property_version":"0",
        ///     "maximum":"1",
        ///     "mutability_config":{
        ///         "description":false,
        ///         "maximum":false,
        ///         "properties":false,
        ///         "royalty":false,
        ///         "uri":false
        ///     },
        ///     "name":"Alice's first token",
        ///     "royalty":{
        ///         "payee_address":"0x3f99ffee67853e129173b9df0e2e9c6af6f08fe2a4417daf43df46ad957a8bbe",
        ///         "royalty_points_denominator":"1000000",
        ///         "royalty_points_numerator":"0"
        ///     },
        ///     "supply":"1",
        ///     "uri":"https://aptos.dev/img/nyan.jpeg"
        /// }
        /// </code>
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="creator">Address of the creator.</param>
        /// <param name="collectionName">Name of the collection.</param>
        /// <param name="tokenName">Name of the token.</param>
        /// <param name="propertyVersion">Version of the token.</param>
        /// <returns>Calls <c>callback</c> function with <c>(TableItemToken, ResponseInfo)</c>: \n
        /// An object the represents the NFT's token metadata - null if the transaction to get a token failed, and a response object that contains the response details.
        /// </returns>
        void GetTokenData(std::function<void (std::shared_ptr<AptosRESTModel::TableItemTokenMetadata>, AptosRESTModel::ResponseInfo)> callback,
            Accounts::AccountAddress creator,
            std::string collectionName,
            std::string tokenName,
            int propertyVersion);
        /// <summary>
        /// Get collection information.
        /// This return a JSON representation that will be parsed by the developer that know the specific return types.
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="creator">Address of the creator.</param>
        /// <param name="collectionName">Name of the collection.</param>
        /// <returns>A JSON string representation of the collection information - null if the request fails.</returns>
        void GetCollection(
            std::function<void(std::string)> callback,
            Accounts::AccountAddress creator,
            std::string collectionName);
        void TransferObject(
            std::function<void(std::string, AptosRESTModel::ResponseInfo)> callback,
            Accounts::Account Owner,
            Accounts::AccountAddress Object,
            Accounts::AccountAddress To);
        /// <summary>
        /// Get a resource of a given type from an account.
        /// NOTE: The response is a complex object of types only known to the developer writing the contracts.
        /// This function return a string and expect the developer to deserialize it into an object.
        /// See <see cref="GetAccountResourceCollection(Action{ResourceCollection, ResponseInfo}, AccountAddress, string)">GetAccountResourceCollection</see> for an example.
        /// </summary>
        /// <param name="callback">Callback function used when response is received.</param>
        /// <param name="accountAddress">Address of the account.</param>
        /// <param name="resourceType">Type of resource being queried for.</param>
        /// <returns>Calls <c>callback</c> function with <c>(bool, long, string)</c>: \n
        /// -- bool: success boolean \n
        /// -- long: - error code, string - JSON response to be deserialized by the consumer of the function\n
        /// -- string: - the response which may contain the resource details</returns>
        void GetAccountResource(
            std::function<void(bool, long, std::string)> callback,
            Accounts::AccountAddress &accountAddress,
            std::string resourceType);

        int ChainId() const;

    private:
       std::string endpoint;
       int m_ChainId;
    };
}
#endif // RESTCLIENT_H
