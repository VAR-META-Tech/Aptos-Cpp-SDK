#ifndef RESOURCECOLLECTION_H
#define RESOURCECOLLECTION_H


#include <iostream>
#include <nlohmann/json.hpp>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {

/// <summary>
/// Represents account resource for a collection.
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/MoveResource
/// </summary>
class APTOS_API ResourceCollection {
public:
    class APTOS_API CollectionEvents {
    public:
        nlohmann::json ToJson() const;
        static CollectionEvents FromJson(const nlohmann::json& jsonData);
    private:
        std::string Counter;
        std::string Guid;
    };

    class APTOS_API TokenData {
    public:
        nlohmann::json ToJson() const;
        static TokenData FromJson(const nlohmann::json& jsonData);
        std::string getHandle() const;

    private:
        std::string Handle;
    };

    class APTOS_API Guid {
    public:
        nlohmann::json ToJson() const;
        static Guid FromJson(const nlohmann::json& jsonData);
    private:
        std::string Addr;
        std::string CreationNum;
    };

    class APTOS_API CollectionData {
    public:
        nlohmann::json ToJson() const;
        static CollectionData FromJson(const nlohmann::json& jsonData);
        std::string getHandle() const;

    private:
        std::string Handle;
    };

    class APTOS_API Data {
    public:
        nlohmann::json ToJson() const;
        static Data FromJson(const nlohmann::json& jsonData);
        TokenData getTokenDataProp() const;

        ResourceCollection::ResourceCollection::CollectionData getCollectionDataProp() const;

    private:
        CollectionData CollectionDataProp;
        CollectionEvents CreateCollectionEvents;
        CollectionEvents CreateTokenDataEvents;
        CollectionEvents MintTokenEvents;
        TokenData TokenDataProp;
    };

    nlohmann::json ToJson() const;
    static ResourceCollection FromJson(const nlohmann::json &jsonData);
    ResourceCollection::Data getDataProp() const;

private:
    std::string Type;
    Data DataProp;
};

}
#endif // RESOURCECOLLECTION_H
