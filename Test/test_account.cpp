//
// Created by Anh NPH on 27/09/2023.
//

#include "gtest/gtest.h"
#include "cryptopp/xed25519.h"
#include <string>
#include "../Accounts/Signature.h"
#include "../Accounts/PublicKey.h"
#include "../Accounts/PrivateKey.h"

using CryptoPP::byte;

// Extended PrivateKey for reference
 std::vector<byte> extendedPrivateKeyBytes = {
        100, 245, 118, 3, 181, 138, 241, 105,
        7, 193, 138, 134, 97, 35, 40, 110,
        28, 188, 232, 151, 144, 97, 53, 88,
        220, 23, 117, 171, 179, 252, 92, 140,
        88, 110, 60, 141, 68, 125, 118, 121,
        34, 46, 19, 144, 51, 227, 130, 2,
        53, 227, 61, 165, 9, 30, 155, 11,
        184, 241, 161, 18, 207, 12, 143, 245
};
std::vector<byte> PrivateKeyBytes = {
        100, 245, 118, 3, 181, 138, 241, 105,
        7, 193, 138, 134, 97, 35, 40, 110,
        28, 188, 232, 151, 144, 97, 53, 88,
        220, 23, 117, 171, 179, 252, 92, 140
};

std::string PrivateKeyHex = "0x64f57603b58af16907c18a866123286e1cbce89790613558dc1775abb3fc5c8c";

std::vector<byte> PrivateKeySerializedOutput =
        {
                32, 100, 245, 118, 3, 181, 138, 241,
                105, 7, 193, 138, 134, 97, 35, 40,
                110, 28, 188, 232, 151, 144, 97, 53,
                88, 220, 23, 117, 171, 179, 252, 92, 140
        };

std::vector<byte> PublicKeyBytes = {
        88, 110, 60, 141, 68, 125, 118, 121,
        34, 46, 19, 144, 51, 227, 130, 2,
        53, 227, 61, 165, 9, 30, 155, 11,
        184, 241, 161, 18, 207, 12, 143, 245
};
std::string PublicKeyHex = "0x586e3c8d447d7679222e139033e3820235e33da5091e9b0bb8f1a112cf0c8ff5";

std::string AccountAddress = "0x9f628c43d1c1c0f54683cf5ccbd2b944608df4ff2649841053b1790a4d7c187d";

std::vector<byte> PublicKeySerializedOutput = {
        32, 88, 110, 60, 141, 68, 125, 118,
        121, 34, 46, 19, 144, 51, 227, 130,
        2, 53, 227, 61, 165, 9, 30, 155,
        11, 184, 241, 161, 18, 207, 12, 143, 245
};

std::vector<byte> PrivateKeyBytesInvalid = {
        100, 245, 118, 3, 181, 138, 241, 105,
        7, 193, 138, 134, 97, 35, 40, 110,
        28, 188, 232, 151, 144, 97, 53, 88,
        220, 23, 117, 171, 179, 252, 92, 140,
        88, 110, 60, 141, 68, 125, 118, 121,
        34, 46, 19, 144, 51, 227, 130, 2,
        53, 227, 61, 165, 9, 30, 155, 11,
};

std::vector<byte> PublicKeyBytesInvalid = {
        88, 110, 60, 141, 68, 125, 118, 121,
        34, 46, 19, 144, 51, 227, 130, 2,
        53, 227, 61, 165, 9, 30, 155, 11,
        184, 241, 161, 18, 207, 12, 143, 245
};

std::string AccountAddressHex = "0x9f628c43d1c1c0f54683cf5ccbd2b944608df4ff2649841053b1790a4d7c187d";
std::string AccountAuthKeyHex = "0x9f628c43d1c1c0f54683cf5ccbd2b944608df4ff2649841053b1790a4d7c187d";

std::vector<byte> MessageUtf8Bytes = {
        87, 69, 76, 67, 79, 77, 69, 32,
        84, 79, 32, 65, 80, 84, 79, 83, 33 };
std::string Message = "WELCOME TO APTOS!";

std::vector<byte> SignatureBytes =
        {
                170, 66, 187, 194, 169, 252, 117, 27,
                238, 238, 59, 49, 43, 132, 82, 196,
                69, 199, 212, 171, 134, 152, 3, 107,
                12, 249, 242, 228, 106, 9, 139, 176,
                44, 54, 159, 188, 141, 254, 253, 35,
                26, 18, 141, 138, 75, 185, 173, 207,
                228, 94, 7, 24, 139, 117, 140, 58,
                211, 152, 215, 248, 78, 130, 239, 5
        };

Signature signatureObject(SignatureBytes);

std::string SignatureHex = "0xaa42bbc2a9fc751beeee3b312b8452c445c7d4ab8698036b0cf9f2e46a098bb02c369fbc8dfefd231a128d8a4bb9adcfe45e07188b758c3ad398d7f84e82ef05";

byte SignatureSerializedOutput[] ={
        64, 170, 66, 187, 194, 169, 252, 117,
        27, 238, 238, 59, 49, 43, 132, 82,
        196, 69, 199, 212, 171, 134, 152, 3,
        107, 12, 249, 242, 228, 106, 9, 139,
        176, 44, 54, 159, 188, 141, 254, 253,
        35, 26, 18, 141, 138, 75, 185, 173,
        207, 228, 94, 7, 24, 139, 117, 140,
        58, 211, 152, 215, 248, 78, 130, 239, 5 };

TEST(AccountTest, GeneratePrivateKeysWithBytesSuccess) {
    PrivateKey* privateKey = new PrivateKey(PrivateKeyBytes);
    ASSERT_TRUE(privateKey->KeyBytes().data() != nullptr);
    ASSERT_EQ(32, privateKey->KeyBytes().size());
    ASSERT_EQ(privateKey->KeyBytes(), PrivateKeyBytes);

    std::string privateKeyHex = privateKey->Key();
    ASSERT_EQ(privateKeyHex, PrivateKeyHex);
}
TEST(AccountTest, GenerateKeysWithBytesSuccess)
{
    PrivateKey* privateKey = new PrivateKey(PrivateKeyBytes);
    PublicKey* publicKey = new PublicKey(PublicKeyBytes);

    ASSERT_TRUE(privateKey->KeyBytes().data() != nullptr);
    ASSERT_TRUE(publicKey->KeyBytes().data() != nullptr);

    std::string privateKeyHex = privateKey->ToString();
    std::string publicKeyHex = publicKey->ToString();

    ASSERT_EQ(privateKeyHex, PrivateKeyHex);
    ASSERT_EQ(publicKeyHex, PublicKeyHex);
}

TEST(AccountTest, GenerateKeysWithStringSuccess)
{
    PrivateKey* privateKey = new PrivateKey(PrivateKeyHex);
    PublicKey* publicKey = new PublicKey(PublicKeyHex);

    ASSERT_TRUE(privateKey->KeyBytes().data() != nullptr);
    ASSERT_TRUE(publicKey->KeyBytes().data() != nullptr);

    ASSERT_EQ(privateKey->KeyBytes(), PrivateKeyBytes);
    ASSERT_EQ(publicKey->KeyBytes(), PublicKeyBytes);

    std::string privateKeyHex = privateKey->Key();
    std::string publicKeyHex = publicKey->Key();

    ASSERT_EQ(privateKeyHex, PrivateKeyHex);
    ASSERT_EQ(publicKeyHex, PublicKeyHex);
}

TEST(AccountTest, GeneratePublicKeyFromPrivateKeySuccess)
{
    PrivateKey* privateKey = new PrivateKey(PrivateKeyHex);
    PublicKey publicKey = privateKey->GetPublicKey();

    ASSERT_EQ(PublicKeyBytes, publicKey.KeyBytes());
}

TEST(AccountTest, PrivateKeyFromHexSignSuccess)
{
    PrivateKey * privateKey = new PrivateKey(PrivateKeyHex);
    ASSERT_EQ(privateKey->Key(), PrivateKeyHex);
    ASSERT_EQ(privateKey->KeyBytes(), PrivateKeyBytes);

    Signature signature = privateKey->Sign(MessageUtf8Bytes);
    ASSERT_EQ(signature, signatureObject);
}

TEST(AccountTest, PrivateKeyFromBytesSignSuccess)
{
    PrivateKey privateKey(PrivateKeyBytes);
    ASSERT_EQ(privateKey.Key(), PrivateKeyHex);
    ASSERT_EQ(privateKey.KeyBytes(), PrivateKeyBytes);

    Signature signature = privateKey.Sign(MessageUtf8Bytes);
    ASSERT_EQ(signature, signatureObject);
}

TEST(AccountTest, InvalidKeyGeneration)
{
    ASSERT_THROW( {
                      PrivateKey* privateKey = new PrivateKey(PrivateKeyBytesInvalid);
                      PublicKey* publicKey = new PublicKey(PublicKeyBytesInvalid);
    },std::invalid_argument);
}

