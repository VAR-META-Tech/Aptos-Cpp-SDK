#include "GasEstimation.h"
namespace AptosRESTModel {


nlohmann::json GasEstimation::ToJson() const {
    return nlohmann::json{
        {"deprioritized_gas_estimate", DeprioritizedGasEstimate},
        {"gas_estimate", GasEstimate},
        {"prioritized_gas_estimate", PrioritizedGasEstimate}
    };
}

GasEstimation GasEstimation::FromJson(const nlohmann::json &jsonData) {
    GasEstimation gasEstimation;
    gasEstimation.DeprioritizedGasEstimate = jsonData["deprioritized_gas_estimate"];
    gasEstimation.GasEstimate = jsonData["gas_estimate"];
    gasEstimation.PrioritizedGasEstimate = jsonData.value("prioritized_gas_estimate", 0);

    return gasEstimation;
}

}
