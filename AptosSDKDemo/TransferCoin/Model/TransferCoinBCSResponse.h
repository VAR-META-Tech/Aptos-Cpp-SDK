#ifndef TRANSFERCOINBCSRESPONSE_H
#define TRANSFERCOINBCSRESPONSE_H

#include <nlohmann/json.hpp>

namespace AptosSDKDemo
{
    /// {
    ///     "hash":"0xaa01b18c71e1d217573aadcf5ef54427d1154ef43151bf4ad65a089d38c66da3",
    ///     "sender":"0x9f628c43d1c1c0f54683cf5ccbd2b944608df4ff2649841053b1790a4d7c187d",
    ///     "sequence_number":"3",
    ///     "max_gas_amount":"100000",
    ///     "gas_unit_price":"100",
    ///     "expiration_timestamp_secs":"1686981051",
    ///     "payload":{
    ///         "function":"0x1::aptos_account::transfer_coins",
    ///         "type_arguments":[
    ///             "0x1::aptos_coin::AptosCoin"
    ///         ],
    ///         "arguments":[
    ///             "0xd89fd73ef7c058ccf79fe4c1c38507d580354206a36ae03eea01ddfd3afeef07",
    ///             "1000"
    ///         ],
    ///         "type":"entry_function_payload"
    ///     },
    ///     "signature":{
    ///         "public_key":"0x586e3c8d447d7679222e139033e3820235e33da5091e9b0bb8f1a112cf0c8ff5",
    ///         "signature":"0xbf414d58d3780908e640ee1e18b2c3c408d0f8723a08769ba2f4d6fdcffd72ee5d74d06d293feeb54f3d3949c71735f34ff500dd8e0b3172964bef91b80fbf00",
    ///         "type":"ed25519_signature"
    ///     }
    /// }
class TransferCoinBCSResponse
{
private:
    std::string hash;
    std::string sender;
    std::string sequenceNumber;
    std::string maxGasAmount;
    std::string gasUnitPrice;
    std::string expirationTimestampSecs;

public:
    const std::string& getHash() const;
    void setHash(const std::string& newHash);

    const std::string& getSender() const;
    void setSender(const std::string& newSender);

    const std::string& getSequenceNumber() const;
    void setSequenceNumber(const std::string& newSequenceNumber);

    const std::string& getMaxGasAmount() const;
    void setMaxGasAmount(const std::string& newMaxGasAmount);

    const std::string& getGasUnitPrice() const;
    void setGasUnitPrice(const std::string& newGasUnitPrice);

    const std::string& getExpirationTimestampSecs() const;
    void setExpirationTimestampSecs(const std::string& newExpirationTimestampSecs);

    nlohmann::json to_json() const;
    static TransferCoinBCSResponse from_json(const nlohmann::json& j);
};
}
#endif // TRANSFERCOINBCSRESPONSE_H
