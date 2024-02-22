#include "LedgerInfo.h"
namespace AptosRESTModel {

nlohmann::json LedgerInfo::ToJson() const {
    return nlohmann::json{
        {"chain_id", ChainId},
        {"epoch", Epoch},
        {"ledger_version", LedgerVersion},
        {"oldest_ledger_version", OldestLedgerVersion},
        {"ledger_timestamp", LedgerTimestamp},
        {"node_role", NodeRole},
        {"oldest_block_height", OldestBlockHeight},
        {"block_height", BlockHeight},
        {"git_hash", GitHash}
    };
}

LedgerInfo LedgerInfo::FromJson(const nlohmann::json &jsonData) {
    LedgerInfo ledgerInfo;

    ledgerInfo.ChainId = jsonData["chain_id"];
    ledgerInfo.Epoch = jsonData["epoch"];
    ledgerInfo.LedgerVersion = jsonData["ledger_version"];
    ledgerInfo.OldestLedgerVersion = jsonData["oldest_ledger_version"];
    ledgerInfo.LedgerTimestamp = jsonData["ledger_timestamp"];
    ledgerInfo.NodeRole = jsonData["node_role"];
    ledgerInfo.OldestBlockHeight = jsonData["oldest_block_height"];
    ledgerInfo.BlockHeight = jsonData["block_height"];
    ledgerInfo.GitHash = jsonData["git_hash"];

    return ledgerInfo;
}

int LedgerInfo::getChainId() const
{
    return ChainId;
}

}
