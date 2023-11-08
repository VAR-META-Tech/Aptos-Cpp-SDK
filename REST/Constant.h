#ifndef CONSTANT_H
#define CONSTANT_H

#include <string>

namespace Constants {
extern const std::string MAINNET_BASE_URL;
extern const std::string TESTNET_BASE_URL;
extern const std::string DEVNET_BASE_URL;

extern const std::string APTOS_COIN_TYPE;

extern const int EXPIRATION_TTL;
extern const int GAS_UNIT_PRICE;
extern const int MAX_GAS_AMOUNT;
extern const int TRANSACTION_WAIT_IN_SECONDS;

extern const std::string ED25519_SIGNATURE;
extern const std::string ENTRY_FUNCTION_PAYLOAD;
extern const std::string CREATE_TOKEN_SCRIPT_FUNCTION;
extern const std::string CREATE_COLLECTION_SCRIPT;
extern const std::string DIRECT_TRANSFER_SCRIPT;

extern const std::string TOKEN_TRANSFER_OFFER_SCRIPT;
extern const std::string TOKEN_TRANSFER_CLAIM_SCRIPT;
extern const std::string COIN_TRANSFER_FUNCTION;
extern const std::string APTOS_ASSET_TYPE;
extern const std::string APTOS_OBJECT_TYPE;
extern const std::string APTOS_TRANSFER_CALL;
extern const std::string APTOS_MINT_EVENT;
}


#endif // CONSTANT_H
