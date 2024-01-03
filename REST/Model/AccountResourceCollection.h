#ifndef ACCOUNTRESOURCECOLLECTION_H
#define ACCOUNTRESOURCECOLLECTION_H

#include <string>
#include <nlohmann/json.hpp>

namespace AptosRESTModel {
/// <summary>
/// Represents Account Resource for a Collection
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/MoveResource
/// </summary>
class ResourceCollectionOLD {
public:
    class Id {
    public:
        nlohmann::json ToJson() const;
        static Id FromJson(const nlohmann::json& jsonData);
    private:
        std::string Addr;
        std::string CreationNum;
    };

    class Guid {
    public:
        nlohmann::json ToJson() const;
        static Guid FromJson(const nlohmann::json& jsonData);
    private:
        Id IdProp;
    };

    class TokenData {
    public:
        nlohmann::json ToJson() const;
        static TokenData FromJson(const nlohmann::json& jsonData);
    private:
        std::string Handle;
    };

    class CollectionEvents {
    public:
        nlohmann::json ToJson() const;
        static CollectionEvents FromJson(const nlohmann::json& jsonData);
    private:
        std::string Counter;
        Guid GuidProp;
    };

    class CollectionData {
    public:
        nlohmann::json ToJson() const;
        static CollectionData FromJson(const nlohmann::json& jsonData);
    private:
        std::string Handle;
    };

    class Data {
    public:
        nlohmann::json ToJson() const;
        static Data FromJson(const nlohmann::json& jsonData);
    private:
        CollectionData CollectionDataProp;
        CollectionEvents CreateCollectionEvents;
        CollectionEvents CreateTokenDataEvents;
        CollectionEvents MintTokenEvents;
        TokenData TokenDataProp;
    };

    ResourceCollectionOLD();
    nlohmann::json ToJson() const;
    static ResourceCollectionOLD FromJson(const nlohmann::json &jsonData);

private:
    std::string Type;
    Data DataProp;
};

}
#endif // ACCOUNTRESOURCECOLLECTION_H
