
#include <gtest/gtest.h>
#include "../Accounts/Account.h"

#include "../HDWallet/Wallet.h"
#include "../Accounts/Ed25519Signature.h"
#include "../HDWallet/Utils/Utils.h"

using namespace Aptos;
using namespace Aptos::HDWallet;
using namespace Aptos::Accounts;
static const std::string mnemo = "stadium valid laundry unknown tuition train december camera fiber vault sniff ripple";
static const std::string Message = "WELCOME TO APTOS!";

static const std::vector<uint8_t> SeedNoPhrase = {
    125, 168, 253, 127, 208, 60, 18, 9,
    188, 118, 79, 248, 22, 177, 237, 218,
    150, 207, 109, 18, 216, 194, 161, 200,
    81, 195, 154, 226, 124, 148, 120, 121,
    218, 142, 242, 104, 202, 44, 246, 159,
    208, 250, 42, 58, 204, 203, 89, 114,
    96, 203, 231, 176, 7, 227, 4, 176,
    222, 227, 185, 220, 247, 250, 223, 167 };

static const std::vector<uint8_t> MessageUt8Bytes = {
    87, 69, 76, 67, 79, 77, 69, 32,
    84, 79, 32, 65, 80, 84, 79, 83, 33 };

static const std::vector<uint8_t> SignatureBytes = {
    170, 66, 187, 194, 169, 252, 117, 27,
    238, 238, 59, 49, 43, 132, 82, 196,
    69, 199, 212, 171, 134, 152, 3, 107,
    12, 249, 242, 228, 106, 9, 139, 176,
    44, 54, 159, 188, 141, 254, 253, 35,
    26, 18, 141, 138, 75, 185, 173, 207,
    228, 94, 7, 24, 139, 117, 140, 58,
    211, 152, 215, 248, 78, 130, 239, 5 };

TEST(WalletTest, CreateWallet) {
    Wallet wallet = Wallet(mnemo);
    ASSERT_EQ(bip3x::bytes_data(SeedNoPhrase), wallet.DeriveMnemonicSeed());
}

TEST(WalletTest, SignMessage) {
    Ed25519Signature signatureObject = Ed25519Signature(Utils::ByteVectorToSecBlock(SignatureBytes));
    Wallet wallet = Wallet(mnemo);
    Account acct = wallet.Account();
    Ed25519Signature signature = acct.Sign(Utils::ByteVectorToSecBlock(MessageUt8Bytes));
    EXPECT_EQ(signatureObject, signature);
}

TEST(WalletTest, VerifySignature) {
    Ed25519Signature signatureObject = Ed25519Signature(Utils::ByteVectorToSecBlock(SignatureBytes));
    Wallet wallet = Wallet(mnemo);
    Account acct = wallet.Account();
    bool verify = acct.Verify(Utils::ByteVectorToSecBlock(MessageUt8Bytes), signatureObject);
    EXPECT_TRUE(verify);
}
