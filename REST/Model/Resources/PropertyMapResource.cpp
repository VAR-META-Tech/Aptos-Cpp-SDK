#include "PropertyMapResource.h"

namespace AptosRESTModel {

nlohmann::json PropertyMapResource::ToJson() const {
    nlohmann::json propertyMapResourceJson;
    propertyMapResourceJson["type"] = Type;
    propertyMapResourceJson["data"] = Data.ToJson();
    return propertyMapResourceJson;
}

PropertyMapResource PropertyMapResource::FromJson(const nlohmann::json &propertyMapResourceJson) {
    PropertyMapResource propertyMapResource;
    propertyMapResource.Type = propertyMapResourceJson["type"];
    propertyMapResource.Data = PropertyMapResourceData::FromJson(propertyMapResourceJson["data"]);
    return propertyMapResource;
}

nlohmann::json PropertyMapResourceData::ToJson() const {
    nlohmann::json propertyMapDataJson;
    propertyMapDataJson["inner"] = InnerData.ToJson();
    return propertyMapDataJson;
}

PropertyMapResourceData PropertyMapResourceData::FromJson(const nlohmann::json &propertyMapDataJson) {
    PropertyMapResourceData propertyMapData;
    propertyMapData.InnerData = Inner::FromJson(propertyMapDataJson["inner"]);
    return propertyMapData;
}

nlohmann::json Inner::ToJson() const {
    nlohmann::json innerJson;
    for (const auto& propertyResource : Data) {
        innerJson.push_back(propertyResource.ToJson());
    }
    return innerJson;
}

Inner Inner::FromJson(const nlohmann::json &innerJson) {
    Inner inner;
    for (const auto& item : innerJson) {
        inner.Data.push_back(PropertyResource::FromJson(item));
    }
    return inner;
}

nlohmann::json PropertyResource::ToJson() const {
    nlohmann::json propertyResourceJson;
    propertyResourceJson["key"] = Key;
    propertyResourceJson["value"] = Value.ToJson();
    return propertyResourceJson;
}

PropertyResource PropertyResource::FromJson(const nlohmann::json &propertyResourceJson) {
    PropertyResource propertyResource;
    propertyResource.Key = propertyResourceJson["key"];
    propertyResource.Value = PropertyValue::FromJson(propertyResourceJson["value"]);
    return propertyResource;
}

nlohmann::json PropertyValue::ToJson() const {
    nlohmann::json propertyValueJson;
    propertyValueJson["type"] = Type;
    propertyValueJson["value"] = Value;
    return propertyValueJson;
}

PropertyValue PropertyValue::FromJson(const nlohmann::json &propertyValueJson) {
    PropertyValue propertyValue;
    propertyValue.Type = propertyValueJson["type"];
    propertyValue.Value = propertyValueJson["value"];
    return propertyValue;
}

}