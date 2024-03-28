#ifndef ACCOUNTRESOURCETOKENSTORE_H
#define ACCOUNTRESOURCETOKENSTORE_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {

/// <summary>
/// Represents Account Resource for Aptos Coin
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/MoveResource
/// </summary>
class APTOS_API AccountResourceTokenStore {
public:
    class APTOS_API Tokens {
    public:
        nlohmann::json ToJson() const;
        static Tokens FromJson(const nlohmann::json& jsonData);
        std::string getHandle() const;

    private:
        std::string Handle;
    };


    class APTOS_API Id {
    public:
        nlohmann::json ToJson() const;
        static Id FromJson(const nlohmann::json& jsonData);
    private:
        std::string Addr;
        std::string CreationNum;
    };

    class APTOS_API Guid {
    public:
        nlohmann::json ToJson() const;
        static Guid FromJson(const nlohmann::json& jsonData);
    private:
        Id IdProp;
    };

    class APTOS_API WithdrawEvents {
    public:
        nlohmann::json ToJson() const;
        static WithdrawEvents FromJson(const nlohmann::json& jsonData);
    private:
        std::string Counter;
        Guid GuidProp;
    };

    class APTOS_API MutateTokenPropertyEvents {
    public:
        nlohmann::json ToJson() const;
        static MutateTokenPropertyEvents FromJson(const nlohmann::json& jsonData);
    private:
        std::string Counter;
        Guid GuidProp;
    };

    class APTOS_API DespositEvents {
    public:
        nlohmann::json ToJson() const;
        static DespositEvents FromJson(const nlohmann::json& jsonData);
    private:
        std::string Counter;
        Guid GuidProp;
    };

    class APTOS_API BurnEvents {
    public:
        nlohmann::json ToJson() const;
        static BurnEvents FromJson(const nlohmann::json& jsonData);
    private:
        std::string Counter;
        Guid GuidProp;
    };

    class APTOS_API Data {
    public:
        nlohmann::json ToJson() const;
        static Data FromJson(const nlohmann::json& jsonData);
        Tokens getTokensProp() const;

    private:
        BurnEvents BurnEvent;
        DespositEvents DepositEvents;
        std::string DirectTransfer;
        MutateTokenPropertyEvents MutateTokenPropertyEventsProp;
        Tokens TokensProp;
    };

    AccountResourceTokenStore() = default;
    nlohmann::json ToJson() const;
    static AccountResourceTokenStore FromJson(const nlohmann::json &jsonData);
    Data getDataProp() const;

private:
    std::string Type;
    Data DataProp;
};

}
#endif // ACCOUNTRESOURCETOKENSTORE_H
