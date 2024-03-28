#ifndef RESOURCEEVENT_H
#define RESOURCEEVENT_H
#include <iostream>
#include <nlohmann/json.hpp>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {

/// <summary>
/// "transfer_events":{
///     "counter":"0",
///     "guid":{
///         "id":{
///             "addr":"0xe0f9ff3281477d787365fec2531ba0ffc01b272ee692dfd2eb49839d893e9771",
///             "creation_num":"1125899906842624"
///         }
///     }
/// }
/// </summary>
class APTOS_API ResourceEvent {
public:
    class APTOS_API EventId {
    public:
        nlohmann::json ToJson() const;
        static EventId FromJson(const nlohmann::json& eventIdJson);
    private:
        std::string Address;
        std::string CreationNumber;
    };

    class APTOS_API GUid {
    public:
        nlohmann::json ToJson() const;
        static GUid FromJson(const nlohmann::json& guidJson);
    private:
        EventId Id;
    };

    nlohmann::json ToJson() const;
    static ResourceEvent FromJson(const nlohmann::json& resourceEventJson);
private:
    std::string Counter;
    GUid GUidProp;
};

}
#endif // RESOURCEEVENT_H
