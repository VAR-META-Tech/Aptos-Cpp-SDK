#include "TokenResource.h"

namespace AptosRESTModel {

nlohmann::json TokenResource::ToJson() const {
    nlohmann::json tokenResourceJson;
    tokenResourceJson["type"] = Type;
    tokenResourceJson["data"] = Data.ToJson();
    return tokenResourceJson;
}

TokenResource TokenResource::FromJson(const nlohmann::json &tokenResourceJson) {
    TokenResource tokenResource;
    tokenResource.Type = tokenResourceJson["type"];
    tokenResource.Data = TokenResourceData::FromJson(tokenResourceJson["data"]);
    return tokenResource;
}

std::string TokenResource::getType() const
{
    return Type;
}

void TokenResource::setType(const std::string &newType)
{
    Type = newType;
}

TokenResourceData TokenResource::getData() const
{
    return Data;
}

void TokenResource::setData(const TokenResourceData &newData)
{
    Data = newData;
}

nlohmann::json TokenResourceData::ToJson() const {
    nlohmann::json tokenDataJson;
    tokenDataJson["collection"] = CollectionProp.ToJson();
    tokenDataJson["description"] = Description;
    tokenDataJson["index"] = Index;
    tokenDataJson["mutation_events"] = MutationEvents.ToJson();
    tokenDataJson["name"] = Name;
    tokenDataJson["uri"] = Uri;
    return tokenDataJson;
}

TokenResourceData TokenResourceData::FromJson(const nlohmann::json &tokenDataJson) {
    TokenResourceData tokenData;
    tokenData.CollectionProp = Collection::FromJson(tokenDataJson["collection"]);
    tokenData.Description = tokenDataJson["description"];
    tokenData.Index = tokenDataJson["index"];
    tokenData.MutationEvents = ResourceEvent::FromJson(tokenDataJson["mutation_events"]);
    tokenData.Name = tokenDataJson["name"];
    tokenData.Uri = tokenDataJson["uri"];
    return tokenData;
}

Collection TokenResourceData::getCollectionProp() const
{
    return CollectionProp;
}

std::string TokenResourceData::getDescription() const
{
    return Description;
}

std::string TokenResourceData::getIndex() const
{
    return Index;
}

std::string TokenResourceData::getName() const
{
    return Name;
}

std::string TokenResourceData::getUri() const
{
    return Uri;
}

nlohmann::json Collection::ToJson() const {
    nlohmann::json collectionJson;
    collectionJson["inner"] = Inner;
    return collectionJson;
}

Collection Collection::FromJson(const nlohmann::json &collectionJson) {
    Collection collection;
    collection.Inner = collectionJson["inner"];
    return collection;
}

std::string Collection::getInner() const
{
    return Inner;
}

}
