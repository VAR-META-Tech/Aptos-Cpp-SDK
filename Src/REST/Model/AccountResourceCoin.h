#ifndef ACCOUNTRESOURCECOIN_H
#define ACCOUNTRESOURCECOIN_H

#include <string>
#include <nlohmann/json.hpp>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {
/// <summary>
/// Represents Account Resource for Aptos Coin
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/MoveResource
/// </summary>
class APTOS_API AccountResourceCoin {
public:
    class APTOS_API Id {
    public:
        Id();
        static AccountResourceCoin::Id FromJson(const nlohmann::json& jsonData);
        nlohmann::json ToJson() const;
    private:
        std::string m_addr;
        std::string m_creationNum;
    };

    class APTOS_API Guid {
    public:
        Guid();
        static AccountResourceCoin::Guid FromJson(const nlohmann::json& jsonData);
        nlohmann::json ToJson() const;
    private:
        Id m_idProp;
    };

    class APTOS_API Coin {
    public:
        Coin();
        static AccountResourceCoin::Coin FromJson(const nlohmann::json& jsonData);
        nlohmann::json ToJson() const;
        std::string value() const;

    private:
        std::string m_value;
    };

    class APTOS_API DespositEvents {
    public:
        DespositEvents();
        static AccountResourceCoin::DespositEvents FromJson(const nlohmann::json& jsonData);
        nlohmann::json ToJson() const;
    private:
        std::string m_counter;
        Guid m_guidProp;
    };

    class APTOS_API WithdrawEvents {
    public:
        WithdrawEvents();
        static AccountResourceCoin::WithdrawEvents FromJson(const nlohmann::json& jsonData);
        nlohmann::json ToJson() const;
    private:
        std::string m_counter;
        Guid m_guidProp;
    };

    class APTOS_API Data {
    public:
        Data() {}
        static AccountResourceCoin::Data FromJson(const nlohmann::json& jsonData);
        nlohmann::json ToJson() const;
        Coin coinProp() const;

    private:
        Coin m_coinProp;
        DespositEvents m_depositEvents;
        bool m_frozen;
        WithdrawEvents m_withdrawEvents;
    };

    AccountResourceCoin();
    static AccountResourceCoin FromJson(const nlohmann::json &jsonData);
    nlohmann::json ToJson() const;
    AccountResourceCoin::Data dataProp() const;
private:
    std::string m_type;
    Data m_dataProp;
};
}
#endif // ACCOUNTRESOURCECOIN_H
