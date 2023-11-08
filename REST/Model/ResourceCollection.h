#ifndef RESOURCECOLLECTION_H
#define RESOURCECOLLECTION_H


#include <iostream>
#include <nlohmann/json.hpp>
namespace AptosRESTModel {

/// <summary>
/// Represents account resource for a collection.
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/MoveResource
/// </summary>
class ResourceCollection {
public:
    class CollectionEvents {
    public:
        nlohmann::json ToJson() const;
        static CollectionEvents FromJson(const nlohmann::json& jsonData);
    private:
        std::string Counter;
        std::string Guid;
    };

    class TokenData {
    public:
        nlohmann::json ToJson() const;
        static TokenData FromJson(const nlohmann::json& jsonData);
    private:
        std::string Handle;
    };

    class Guid {
    public:
        nlohmann::json ToJson() const;
        static Guid FromJson(const nlohmann::json& jsonData);
    private:
        std::string Addr;
        std::string CreationNum;
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

    nlohmann::json ToJson() const;
    static ResourceCollection FromJson(const std::string& jsonStr);
private:
    std::string Type;
    Data DataProp;
};

}
#endif // RESOURCECOLLECTION_H
