#include "CollectionResource.h"

namespace AptosRESTModel {

nlohmann::json CollectionResource::ToJson() const {
    nlohmann::json collectionResourceJson;
    collectionResourceJson["type"] = Type;
    collectionResourceJson["data"] = Data.ToJson();
    return collectionResourceJson;
}

CollectionResource CollectionResource::FromJson(const nlohmann::json &collectionResourceJson) {
    CollectionResource collectionResource;
    collectionResource.Type = collectionResourceJson["type"];
    collectionResource.Data = CollectionResourceData::FromJson(collectionResourceJson["data"]);
    return collectionResource;
}

CollectionResourceData CollectionResource::getData() const
{
    return Data;
}

void CollectionResource::setData(const CollectionResourceData &newData)
{
    Data = newData;
}

std::string CollectionResource::getType() const
{
    return Type;
}

void CollectionResource::setType(const std::string &newType)
{
    Type = newType;
}

nlohmann::json CollectionResourceData::ToJson() const {
    nlohmann::json collectionDataJson;
    collectionDataJson["creator"] = Creator;
    collectionDataJson["description"] = Description;
    collectionDataJson["mutation_events"] = MutationEvents.ToJson();
    collectionDataJson["name"] = Name;
    collectionDataJson["uri"] = Uri;
    return collectionDataJson;
}

CollectionResourceData CollectionResourceData::FromJson(const nlohmann::json &collectionDataJson) {
    CollectionResourceData collectionData;
    collectionData.Creator = collectionDataJson["creator"];
    collectionData.Description = collectionDataJson["description"];
    collectionData.MutationEvents = ResourceEvent::FromJson(collectionDataJson["mutation_events"]);
    collectionData.Name = collectionDataJson["name"];
    collectionData.Uri = collectionDataJson["uri"];
    return collectionData;
}

std::string CollectionResourceData::getCreator() const
{
    return Creator;
}

std::string CollectionResourceData::getDescription() const
{
    return Description;
}

std::string CollectionResourceData::getName() const
{
    return Name;
}

std::string CollectionResourceData::getUri() const
{
    return Uri;
}

}
