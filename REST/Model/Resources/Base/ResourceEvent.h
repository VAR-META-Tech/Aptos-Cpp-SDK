#ifndef RESOURCEEVENT_H
#define RESOURCEEVENT_H
#include <iostream>
#include <nlohmann/json.hpp>
namespace AptosRESTModel {

class ResourceEvent {
public:
    class EventId {
    public:
        nlohmann::json ToJson() const;
        static EventId FromJson(const nlohmann::json& eventIdJson);
    private:
        std::string Address;
        std::string CreationNumber;
    };

    class GUid {
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
