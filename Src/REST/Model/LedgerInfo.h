#ifndef LEDGERINFO_H
#define LEDGERINFO_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {

/// <summary>
/// Represents a gas estimation response.
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/operations/get_ledger_info
/// </summary>
class APTOS_API LedgerInfo {
public:
    nlohmann::json ToJson() const;
    static LedgerInfo FromJson(const nlohmann::json &jsonData);

    int getChainId() const;

private:
    int ChainId;
    std::string Epoch;
    std::string LedgerVersion;
    std::string OldestLedgerVersion;
    std::string LedgerTimestamp;
    std::string NodeRole;
    std::string OldestBlockHeight;
    std::string BlockHeight;
    std::string GitHash;
};

}
#endif // LEDGERINFO_H
