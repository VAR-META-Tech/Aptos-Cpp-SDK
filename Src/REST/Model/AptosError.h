#ifndef APTOSERROR_H
#define APTOSERROR_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {

/// <summary>
/// Represents an error return from REST service
/// </summary>
class APTOS_API AptosError {
public:
    nlohmann::json ToJson() const;
    static AptosError FromJson(const nlohmann::json &jsonData);

private:
    std::string Message;
    std::string ErrorCode;
    std::string VMErrorCode;
};
}
#endif // APTOSERROR_H
