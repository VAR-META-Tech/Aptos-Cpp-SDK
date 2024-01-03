#include "ObjectResource.h"

namespace AptosRESTModel {

nlohmann::json ObjectResource::ToJson() const {
    nlohmann::json objectResourceJson;
    objectResourceJson["type"] = Type;
    objectResourceJson["data"] = Data.ToJson();
    return objectResourceJson;
}

ObjectResource ObjectResource::FromJson(const nlohmann::json &objectResourceJson) {
    ObjectResource objectResource;
    objectResource.Type = objectResourceJson["type"];
    objectResource.Data = ObjectResourceData::FromJson(objectResourceJson["data"]);
    return objectResource;
}

std::string ObjectResource::getType() const
{
    return Type;
}

void ObjectResource::setType(const std::string &newType)
{
    Type = newType;
}

ObjectResourceData ObjectResource::getData() const
{
    return Data;
}

void ObjectResource::setData(const ObjectResourceData &newData)
{
    Data = newData;
}

nlohmann::json ObjectResourceData::ToJson() const {
    nlohmann::json objectDataJson;
    objectDataJson["allow_ungated_transfer"] = AllowUngatedTransfer;
    objectDataJson["guid_creation_num"] = GuidCreationNum;
    objectDataJson["owner"] = Owner;
    objectDataJson["transfer_events"] = TransferEvents.ToJson();
    return objectDataJson;
}

ObjectResourceData ObjectResourceData::FromJson(const nlohmann::json &objectDataJson) {
    ObjectResourceData objectData;
    objectData.AllowUngatedTransfer = objectDataJson["allow_ungated_transfer"];
    objectData.GuidCreationNum = objectDataJson["guid_creation_num"];
    objectData.Owner = objectDataJson["owner"];
    objectData.TransferEvents = ResourceEvent::FromJson(objectDataJson["transfer_events"]);
    return objectData;
}

bool ObjectResourceData::getAllowUngatedTransfer() const
{
    return AllowUngatedTransfer;
}

std::string ObjectResourceData::getOwner() const
{
    return Owner;
}

}
