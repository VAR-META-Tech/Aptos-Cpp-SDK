#include "ResourceBaseListConverter.h"
#include "../CollectionResource.h"
#include "../ObjectResource.h"
#include "../PropertyMapResource.h"
#include "../RoyaltyResource.h"
#include "../TokenResource.h"
#include <memory>

std::vector<std::shared_ptr<AptosRESTModel::IResourceBase> > ResourceBaseListConverter::ReadJson(const nlohmann::json &json) {
    using namespace AptosRESTModel;
    std::vector<std::shared_ptr<IResourceBase>> resources;
    for (const auto& item : json) {
        std::string type = item["type"];
        nlohmann::json dataJson = item["data"];

        if (type == "0x4::collection::Collection") {
            auto collectionRes = std::make_shared<CollectionResource>();
            collectionRes->setType(type);
            CollectionResourceData collectionData = CollectionResourceData::FromJson(dataJson);
            collectionRes->setData(collectionData);
            resources.push_back(collectionRes);
        } else if (type == "0x1::object::ObjectCore") {
            auto objectRes = std::make_shared<ObjectResource>();
            objectRes->setType(type);
            ObjectResourceData objectData = ObjectResourceData::FromJson(dataJson);
            objectRes->setData(objectData);
            resources.push_back(objectRes);
        } else if (type == "0x4::property_map::PropertyMap") {
            auto propMapRes = std::make_shared<PropertyMapResource>();
            propMapRes->setType(type);
            PropertyMapResourceData propMapData = PropertyMapResourceData::FromJson(dataJson);
            propMapRes->setData(propMapData);
            resources.push_back(propMapRes);
        } else if (type == "0x4::royalty::Royalty") {
            auto royaltyRes = std::make_shared<RoyaltyResource>();
            royaltyRes->setType(type);
            RoyaltyResourceData royaltyData = RoyaltyResourceData::FromJson(dataJson);
            royaltyRes->setData(royaltyData);
            resources.push_back(royaltyRes);
        } else if (type == "0x4::token::Token") {
            auto tokenRes = std::make_shared<TokenResource>();
            tokenRes->setType(type);
            TokenResourceData tokenData = TokenResourceData::FromJson(dataJson);
            tokenRes->setData(tokenData);
            resources.push_back(tokenRes);
        }
    }

    return resources;
}
