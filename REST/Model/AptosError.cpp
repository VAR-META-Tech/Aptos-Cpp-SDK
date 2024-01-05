#include "AptosError.h"

namespace AptosRESTModel {

nlohmann::json AptosError::ToJson() const {
    return nlohmann::json{
        {"message", Message},
        {"error_code", ErrorCode},
        {"vm_error_code", VMErrorCode}
    };
}

AptosError AptosError::FromJson(const nlohmann::json &jsonData) {
    AptosError aptosError;
    aptosError.Message = jsonData["message"];
    aptosError.ErrorCode = jsonData["error_code"];
    aptosError.VMErrorCode = jsonData["vm_error_code"];

    return aptosError;
}
}
