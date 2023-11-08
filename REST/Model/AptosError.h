#ifndef APTOSERROR_H
#define APTOSERROR_H

#include <iostream>
#include <nlohmann/json.hpp>
namespace AptosRESTModel {

/// <summary>
/// Represents an error return from REST service
/// </summary>
class AptosError {
public:
    nlohmann::json ToJson() const;
    static AptosError FromJson(const std::string& jsonStr);

private:
    std::string Message;
    std::string ErrorCode;
    std::string VMErrorCode;
};
}
#endif // APTOSERROR_H
