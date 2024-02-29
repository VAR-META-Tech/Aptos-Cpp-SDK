#ifndef RESOURCEBASELISTCONVERTER_H
#define RESOURCEBASELISTCONVERTER_H

#include "IResourceBase.h"

class ResourceBaseListConverter
{
public:
    static std::vector<std::shared_ptr<AptosRESTModel::IResourceBase>> ReadJson(const nlohmann::json& json);
};

#endif // RESOURCEBASELISTCONVERTER_H
