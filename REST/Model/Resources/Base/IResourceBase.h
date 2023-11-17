#ifndef IRESOURCEBASE_H
#define IRESOURCEBASE_H

#include <nlohmann/json.hpp>
namespace AptosRESTModel {

class IResourceBase {
public:
    IResourceBase(const std::string& type);
    IResourceBase();
    nlohmann::json ToJson() const;
    static IResourceBase FromJson(const nlohmann::json& resourceJson);
private:
    std::string Type;
};
}
#endif // IRESOURCEBASE_H