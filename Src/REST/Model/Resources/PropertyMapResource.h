#ifndef PROPERTYMAPRESOURCE_H
#define PROPERTYMAPRESOURCE_H

#include <nlohmann/json.hpp>
#include "Base/ResourceDataBase.h"
#include "Base/IResourceBase.h"

namespace AptosRESTModel {

class APTOS_API PropertyValue {
public:
    nlohmann::json ToJson() const;
    static PropertyValue FromJson(const nlohmann::json& propertyValueJson);
    const std::string &getType() const;

    const std::string &getValue() const;

private:
    std::string Type;
    std::string Value;
};

class APTOS_API PropertyResource {
public:
    nlohmann::json ToJson() const;
    static PropertyResource FromJson(const nlohmann::json& propertyResourceJson);
    const std::string &getKey() const;

    const PropertyValue &getValue() const;

private:
    std::string Key;
    PropertyValue Value;
};

class APTOS_API Inner {
public:
    nlohmann::json ToJson() const;
    static Inner FromJson(const nlohmann::json& innerJson);
    const std::vector<PropertyResource> &getData() const;

private:
    std::vector<PropertyResource> Data;
};

class APTOS_API PropertyMapResourceData : public ResourceDataBase {
public:
    nlohmann::json ToJson() const;
    static PropertyMapResourceData FromJson(const nlohmann::json& propertyMapDataJson);
    const Inner &getInnerData() const;

private:
    Inner InnerData;
};

class APTOS_API PropertyMapResource : public IResourceBase {
public:
    nlohmann::json ToJson() const;
    static PropertyMapResource FromJson(const nlohmann::json& propertyMapResourceJson);
    std::string getType() const override;
    void setType(const std::string &newType);

    PropertyMapResourceData getData() const;
    void setData(const PropertyMapResourceData &newData);

private:
    std::string Type;
    PropertyMapResourceData Data;
};

}
#endif // PROPERTYMAPRESOURCE_H
