#ifndef LEDGERINFO_H
#define LEDGERINFO_H

#include <iostream>
#include <nlohmann/json.hpp>
namespace AptosRESTModel {

/// <summary>
/// Represents a gas estimation response.
/// https://fullnode.mainnet.aptoslabs.com/v1/spec#/operations/get_ledger_info
/// </summary>
class LedgerInfo {
public:
    nlohmann::json ToJson() const;
    static LedgerInfo FromJson(const std::string& jsonStr);

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
