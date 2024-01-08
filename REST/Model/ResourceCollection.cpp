#include "ResourceCollection.h"

namespace AptosRESTModel {

nlohmann::json ResourceCollection::ToJson() const {
    return nlohmann::json{
        {"type", Type},
        {"data", DataProp.ToJson()}
    };
}

ResourceCollection ResourceCollection::FromJson(const nlohmann::json &jsonData) {
    ResourceCollection resourceCollection;
    resourceCollection.Type = jsonData["type"].get<std::string>();
    resourceCollection.DataProp = Data::FromJson(jsonData["data"]);
    return resourceCollection;
}

ResourceCollection::Data ResourceCollection::getDataProp() const
{
    return DataProp;
}

nlohmann::json ResourceCollection::Data::ToJson() const {
    return nlohmann::json{
                          {"collection_data", CollectionDataProp.ToJson()},
        {"create_collection_events", CreateCollectionEvents.ToJson()},
        {"create_token_data_events", CreateTokenDataEvents.ToJson()},
        {"mint_token_events", MintTokenEvents.ToJson()},
        {"token_data", TokenDataProp.ToJson()}
    };
}

ResourceCollection::Data ResourceCollection::Data::FromJson(const nlohmann::json &jsonData) {
    Data data;
    data.CollectionDataProp = CollectionData::FromJson(jsonData["collection_data"]);
    data.CreateCollectionEvents = CollectionEvents::FromJson(jsonData["create_collection_events"]);
    data.CreateTokenDataEvents = CollectionEvents::FromJson(jsonData["create_token_data_events"]);
    data.MintTokenEvents = CollectionEvents::FromJson(jsonData["mint_token_events"]);
    data.TokenDataProp = TokenData::FromJson(jsonData["token_data"]);
    return data;
}

ResourceCollection::CollectionData ResourceCollection::Data::getCollectionDataProp() const
{
    return CollectionDataProp;
}

ResourceCollection::TokenData ResourceCollection::Data::getTokenDataProp() const
{
    return TokenDataProp;
}

nlohmann::json ResourceCollection::CollectionData::ToJson() const {
    return nlohmann::json{
        {"handle", Handle}
    };
}

ResourceCollection::CollectionData ResourceCollection::CollectionData::FromJson(const nlohmann::json &jsonData) {
    CollectionData data;
    data.Handle = jsonData["handle"].get<std::string>();
    return data;
}

std::string ResourceCollection::CollectionData::getHandle() const
{
    return Handle;
}

nlohmann::json ResourceCollection::Guid::ToJson() const {
    return nlohmann::json{
        {"addr", Addr},
        {"creation_num", CreationNum}
    };
}

ResourceCollection::Guid ResourceCollection::Guid::FromJson(const nlohmann::json &jsonData) {
    Guid guid;
    guid.Addr = jsonData["addr"].get<std::string>();
    guid.CreationNum = jsonData["creation_num"].get<std::string>();
    return guid;
}

nlohmann::json ResourceCollection::TokenData::ToJson() const {
    return nlohmann::json{
        {"handle", Handle}
    };
}

ResourceCollection::TokenData ResourceCollection::TokenData::FromJson(const nlohmann::json &jsonData) {
    TokenData data;
    data.Handle = jsonData["handle"].get<std::string>();
    return data;
}

std::string ResourceCollection::TokenData::getHandle() const
{
    return Handle;
}

nlohmann::json ResourceCollection::CollectionEvents::ToJson() const {
    return nlohmann::json{
        {"counter", Counter},
        {"guid", Guid}
    };
}

ResourceCollection::CollectionEvents ResourceCollection::CollectionEvents::FromJson(const nlohmann::json &jsonData) {
    CollectionEvents events;
    events.Counter = jsonData["counter"].get<std::string>();
    events.Guid = jsonData["guid"].dump();
    return events;
}

}
