#ifndef COLLECTIONRESOURCE_H
#define COLLECTIONRESOURCE_H
#include <nlohmann/json.hpp>
#include "Base/ResourceDataBase.h"
#include "Base/IResourceBase.h"
#include "Base/ResourceEvent.h"

namespace AptosRESTModel {
class APTOS_API CollectionResourceData : public ResourceDataBase {
public:
    nlohmann::json ToJson() const;
    static CollectionResourceData FromJson(const nlohmann::json& collectionDataJson);
    std::string getCreator() const;
    std::string getDescription() const;
    std::string getName() const;
    std::string getUri() const;

private:
    std::string Creator;
    std::string Description;
    ResourceEvent MutationEvents;
    std::string Name;
    std::string Uri;
};

/// <summary>
/// {
///     "type":"0x4::collection::Collection",
///     "data":{
///         "creator":"0x5a5a71a09e33e6cefbc084c41a854ba440e5ecf304158482f606a00d716afed8",
///         "description":"Alice's simple collection",
///         "mutation_events":{
///             "counter":"0",
///             "guid":{
///                 "id":{
///                     "addr":"0xe0f9ff3281477d787365fec2531ba0ffc01b272ee692dfd2eb49839d893e9771",
///                     "creation_num":"1125899906842627"
///                 }
///             }
///         },
///         "name":"Alice's",
///         "uri":"https://aptos.dev"
///     }
/// }
/// </summary>
class APTOS_API CollectionResource : public IResourceBase{
public:
    nlohmann::json ToJson() const;
    static CollectionResource FromJson(const nlohmann::json& collectionResourceJson);
    CollectionResourceData getData() const;
    void setData(const CollectionResourceData &newData);

    std::string getType() const override;
    void setType(const std::string &newType);

private:
    std::string Type;
    CollectionResourceData Data;
};
}

#endif // COLLECTIONRESOURCE_H
