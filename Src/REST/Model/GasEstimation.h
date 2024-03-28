#ifndef GASESTIMATION_H
#define GASESTIMATION_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {

/// <summary>
/// Represents a gas estimation response.
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/schemas/GasEstimation
/// </summary>
class APTOS_API GasEstimation {
public:
    nlohmann::json ToJson() const;
    static GasEstimation FromJson(const nlohmann::json &jsonData);

private:
    int DeprioritizedGasEstimate;
    int GasEstimate;
    int PrioritizedGasEstimate;
};

}
#endif // GASESTIMATION_H
