#include "AccountResourceCollection.h"
#include "iostream"

namespace AptosRESTModel {

nlohmann::json ResourceCollectionOLD::ToJson() const {
    return nlohmann::json{
        {"type", Type},
        {"data", DataProp.ToJson()}
    };
}

ResourceCollectionOLD ResourceCollectionOLD::FromJson(const nlohmann::json &jsonData) {
    ResourceCollectionOLD resourceCollection;
    resourceCollection.Type = jsonData["type"];
    resourceCollection.DataProp = Data::FromJson(jsonData["data"]);

    return resourceCollection;
}

nlohmann::json ResourceCollectionOLD::Data::ToJson() const {
    return nlohmann::json{
        {"collection_data", CollectionDataProp.ToJson()},
        {"create_collection_events", CreateCollectionEvents.ToJson()},
        {"create_token_data_events", CreateTokenDataEvents.ToJson()},
        {"mint_token_events", MintTokenEvents.ToJson()},
        {"token_data", TokenDataProp.ToJson()}
    };
}

ResourceCollectionOLD::Data ResourceCollectionOLD::Data::FromJson(const nlohmann::json &jsonData) {
    Data data;
    data.CollectionDataProp = CollectionData::FromJson(jsonData["collection_data"]);
    data.CreateCollectionEvents = CollectionEvents::FromJson(jsonData["create_collection_events"]);
    data.CreateTokenDataEvents = CollectionEvents::FromJson(jsonData["create_token_data_events"]);
    data.MintTokenEvents = CollectionEvents::FromJson(jsonData["mint_token_events"]);
    data.TokenDataProp = TokenData::FromJson(jsonData["token_data"]);
    return data;
}

nlohmann::json ResourceCollectionOLD::CollectionData::ToJson() const {
    return nlohmann::json{
        {"handle", Handle}
    };
}

ResourceCollectionOLD::CollectionData ResourceCollectionOLD::CollectionData::FromJson(const nlohmann::json &jsonData) {
    CollectionData data;
    data.Handle = jsonData["handle"];
    return data;
}

nlohmann::json ResourceCollectionOLD::TokenData::ToJson() const {
    return nlohmann::json{
        {"handle", Handle}
    };
}

ResourceCollectionOLD::TokenData ResourceCollectionOLD::TokenData::FromJson(const nlohmann::json &jsonData) {
    TokenData data;
    data.Handle = jsonData["handle"];
    return data;
}

nlohmann::json ResourceCollectionOLD::Id::ToJson() const {
    return nlohmann::json{
        {"addr", Addr},
        {"creation_num", CreationNum}
    };
}

ResourceCollectionOLD::Id ResourceCollectionOLD::Id::FromJson(const nlohmann::json &jsonData) {
    Id id;
    id.Addr = jsonData["addr"];
    id.CreationNum = jsonData["creation_num"];
    return id;
}

nlohmann::json ResourceCollectionOLD::Guid::ToJson() const {
    return nlohmann::json{
        {"id", IdProp.ToJson()}
    };
}

ResourceCollectionOLD::Guid ResourceCollectionOLD::Guid::FromJson(const nlohmann::json &jsonData) {
    Guid guid;
    guid.IdProp = Id::FromJson(jsonData["id"]);
    return guid;
}

nlohmann::json ResourceCollectionOLD::CollectionEvents::ToJson() const {
    return nlohmann::json{
        {"counter", Counter},
        {"guid", GuidProp.ToJson()}
    };
}

ResourceCollectionOLD::CollectionEvents ResourceCollectionOLD::CollectionEvents::FromJson(const nlohmann::json &jsonData) {
    CollectionEvents events;
    events.Counter = jsonData["counter"];
    events.GuidProp = Guid::FromJson(jsonData["guid"]);
    return events;
}

}
