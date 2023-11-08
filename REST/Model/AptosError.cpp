#include "AptosError.h"

namespace AptosRESTModel {

nlohmann::json AptosError::ToJson() const {
    return nlohmann::json{
        {"message", Message},
        {"error_code", ErrorCode},
        {"vm_error_code", VMErrorCode}
    };
}

AptosError AptosError::FromJson(const std::string &jsonStr) {
    AptosError aptosError;

    try {
        nlohmann::json jsonData = nlohmann::json::parse(jsonStr);
        aptosError.Message = jsonData["message"];
        aptosError.ErrorCode = jsonData["error_code"];
        aptosError.VMErrorCode = jsonData["vm_error_code"];
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }

    return aptosError;
}
}
