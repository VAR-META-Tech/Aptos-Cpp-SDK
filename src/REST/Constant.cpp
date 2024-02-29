#include "Constant.h"

const std::string Constants::MAINNET_BASE_URL = "https://fullnode.mainnet.aptoslabs.com";
const std::string Constants::TESTNET_BASE_URL = "https://fullnode.testnet.aptoslabs.com";
const std::string Constants::DEVNET_BASE_URL = "https://fullnode.devnet.aptoslabs.com";

const std::string Constants::APTOS_COIN_TYPE = "0x1::coin::CoinStore%3C0x1::aptos_coin::AptosCoin%3E";

const int Constants::EXPIRATION_TTL = 600;
const int Constants::GAS_UNIT_PRICE = 100;
const int Constants::MAX_GAS_AMOUNT = 100000;
const int Constants::TRANSACTION_WAIT_IN_SECONDS = 20;

const std::string Constants::ED25519_SIGNATURE = "ed25519_signature";
const std::string Constants::ENTRY_FUNCTION_PAYLOAD = "entry_function_payload";
const std::string Constants::CREATE_TOKEN_SCRIPT_FUNCTION = "0x3::token::create_token_script";
const std::string Constants::CREATE_COLLECTION_SCRIPT = "0x3::token::create_collection_script";
const std::string Constants::DIRECT_TRANSFER_SCRIPT = "0x3::token::direct_transfer_script";

const std::string Constants::TOKEN_TRANSFER_OFFER_SCRIPT = "0x3::token_transfers::offer_script";
const std::string Constants::TOKEN_TRANSFER_CLAIM_SCRIPT = "0x3::token_transfers::claim_script";
const std::string Constants::COIN_TRANSFER_FUNCTION = "0x1::aptos_account::transfer";
const std::string Constants::APTOS_ASSET_TYPE = "0x1::aptos_coin::AptosCoin";
const std::string Constants::APTOS_OBJECT_TYPE = "0x1::object";
const std::string Constants::APTOS_TRANSFER_CALL = "transfer_call";
const std::string Constants::APTOS_MINT_EVENT = "0x4::collection::Mint";

