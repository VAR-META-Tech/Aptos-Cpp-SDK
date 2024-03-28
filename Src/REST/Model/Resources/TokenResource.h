#ifndef TOKENRESOURCE_H
#define TOKENRESOURCE_H

#include <nlohmann/json.hpp>
#include "Base/ResourceDataBase.h"
#include "Base/IResourceBase.h"
#include "Base/ResourceEvent.h"

namespace AptosRESTModel {
class APTOS_API Collection {
public:
    nlohmann::json ToJson() const;
    static Collection FromJson(const nlohmann::json& collectionJson);
    std::string getInner() const;

private:
    std::string Inner;
};

class APTOS_API TokenResourceData : public ResourceDataBase {
public:
    nlohmann::json ToJson() const;
    static TokenResourceData FromJson(const nlohmann::json& tokenDataJson);
    Collection getCollectionProp() const;
    std::string getDescription() const;
    std::string getIndex() const;
    std::string getName() const;
    std::string getUri() const;

private:
    Collection CollectionProp;
    std::string Description;
    std::string Index;
    ResourceEvent MutationEvents;
    std::string Name;
    std::string Uri;
};

class APTOS_API TokenResource : public IResourceBase {
public:
    nlohmann::json ToJson() const;
    static TokenResource FromJson(const nlohmann::json& tokenResourceJson);
    std::string getType() const override;
    void setType(const std::string &newType);

    TokenResourceData getData() const;
    void setData(const TokenResourceData &newData);

private:
    std::string Type;
    TokenResourceData Data;
};
}

#endif // TOKENRESOURCE_H
