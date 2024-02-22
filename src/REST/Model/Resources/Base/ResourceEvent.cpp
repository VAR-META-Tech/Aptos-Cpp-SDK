#include "ResourceEvent.h"

namespace AptosRESTModel {

nlohmann::json ResourceEvent::ToJson() const {
    nlohmann::json resourceEventJson;
    resourceEventJson["counter"] = Counter;
    resourceEventJson["guid"] = GUidProp.ToJson();
    return resourceEventJson;
}

ResourceEvent ResourceEvent::FromJson(const nlohmann::json &resourceEventJson) {
    ResourceEvent resourceEvent;
    resourceEvent.Counter = resourceEventJson["counter"];
    resourceEvent.GUidProp = GUid::FromJson(resourceEventJson["guid"]);
    return resourceEvent;
}

nlohmann::json ResourceEvent::GUid::ToJson() const {
    nlohmann::json guidJson;
    guidJson["id"] = Id.ToJson();
    return guidJson;
}

ResourceEvent::GUid ResourceEvent::GUid::FromJson(const nlohmann::json &guidJson) {
    GUid guid;
    guid.Id = EventId::FromJson(guidJson["id"]);
    return guid;
}

nlohmann::json ResourceEvent::EventId::ToJson() const {
    nlohmann::json eventIdJson;
    eventIdJson["addr"] = Address;
    eventIdJson["creation_num"] = CreationNumber;
    return eventIdJson;
}

ResourceEvent::EventId ResourceEvent::EventId::FromJson(const nlohmann::json &eventIdJson) {
    EventId eventId;
    eventId.Address = eventIdJson["addr"];
    eventId.CreationNumber = eventIdJson["creation_num"];
    return eventId;
}
}
