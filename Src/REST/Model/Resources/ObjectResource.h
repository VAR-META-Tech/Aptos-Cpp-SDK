#ifndef OBJECTRESOURCE_H
#define OBJECTRESOURCE_H
#include <nlohmann/json.hpp>
#include "Base/ResourceDataBase.h"
#include "Base/ResourceEvent.h"
#include "Base/IResourceBase.h"
namespace AptosRESTModel {

class APTOS_API ObjectResourceData : public ResourceDataBase {
public:
    nlohmann::json ToJson() const;
    static ObjectResourceData FromJson(const nlohmann::json& objectDataJson);
    bool getAllowUngatedTransfer() const;
    std::string getOwner() const;
private:
    bool AllowUngatedTransfer;
    std::string GuidCreationNum;
    std::string Owner;
    ResourceEvent TransferEvents;
};

/// <summary>
///
/// {
///     "type":"0x1::object::ObjectCore",
///     "data":{
///         "allow_ungated_transfer":false,
///         "guid_creation_num":"1125899906842628",
///         "owner":"0x5a5a71a09e33e6cefbc084c41a854ba440e5ecf304158482f606a00d716afed8",
///         "transfer_events":{
///             "counter":"0",
///             "guid":{
///                 "id":{
///                     "addr":"0xe0f9ff3281477d787365fec2531ba0ffc01b272ee692dfd2eb49839d893e9771",
///                     "creation_num":"1125899906842624"
///                 }
///             }
///         }
///     }
/// }
/// </summary>
///
class APTOS_API ObjectResource : public IResourceBase{
public:
    nlohmann::json ToJson() const;
    static ObjectResource FromJson(const nlohmann::json& objectResourceJson);
    std::string getType() const override; 
    void setType(const std::string &newType);

    ObjectResourceData getData() const;
    void setData(const ObjectResourceData &newData);

private:
    std::string Type;
    ObjectResourceData Data;
};

}
#endif // OBJECTRESOURCE_H
