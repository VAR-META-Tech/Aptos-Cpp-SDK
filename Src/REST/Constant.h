#ifndef CONSTANT_H
#define CONSTANT_H

#include <string>
#include "BCS/BCSTypes.h"

/// <summary>
/// A set of constants used globably across
/// </summary>
namespace Constants {
APTOS_API extern const std::string MAINNET_BASE_URL;
APTOS_API extern const std::string TESTNET_BASE_URL;
APTOS_API extern const std::string DEVNET_BASE_URL;

APTOS_API extern const std::string APTOS_COIN_TYPE;

APTOS_API extern const int EXPIRATION_TTL;
APTOS_API extern const int GAS_UNIT_PRICE;
APTOS_API extern const int MAX_GAS_AMOUNT;
APTOS_API extern const int TRANSACTION_WAIT_IN_SECONDS;

APTOS_API extern const std::string ED25519_SIGNATURE;
APTOS_API extern const std::string ENTRY_FUNCTION_PAYLOAD;
APTOS_API extern const std::string CREATE_TOKEN_SCRIPT_FUNCTION;
APTOS_API extern const std::string CREATE_COLLECTION_SCRIPT;
APTOS_API extern const std::string DIRECT_TRANSFER_SCRIPT;

APTOS_API extern const std::string TOKEN_TRANSFER_OFFER_SCRIPT;
APTOS_API extern const std::string TOKEN_TRANSFER_CLAIM_SCRIPT;
APTOS_API extern const std::string COIN_TRANSFER_FUNCTION;
APTOS_API extern const std::string APTOS_ASSET_TYPE;
APTOS_API extern const std::string APTOS_OBJECT_TYPE;
APTOS_API extern const std::string APTOS_TRANSFER_CALL;
APTOS_API extern const std::string APTOS_MINT_EVENT;
}


#endif // CONSTANT_H
