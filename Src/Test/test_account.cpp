//
// Created by Anh NPH on 27/09/2023.
//

#include "gtest/gtest.h"
#include <string>
#define private public
#include "../Accounts/Ed25519Signature.h"
#include "../Accounts/Ed25519/PublicKey.h"
#include "../Accounts/Ed25519/PrivateKey.h"
#include "../Accounts/AccountAddress.h"
#include "../BCS/BCS.h"
#include "../Accounts/Multipublickey.h"
#include "../HDWallet/Utils/Utils.h"
#include "../Accounts/Multisignature.h"
#include "../Accounts/Account.h"
#include "../Accounts/AuthenticationKey.h"
#include "../Accounts/Types/MultiEd25519PublicKey.h"
#include "../Accounts/Ed25519Bip32.h"

using namespace Aptos::BCS;
using namespace Aptos::Accounts;
using namespace Aptos;
using namespace Aptos::Accounts::Ed25519;

// Extended PrivateKey for reference
CryptoPP::SecByteBlock g_extendedPrivateKeyBytes = Utils::ByteVectorToSecBlock({100, 245, 118, 3, 181, 138, 241, 105,
                                                                                7, 193, 138, 134, 97, 35, 40, 110,
                                                                                28, 188, 232, 151, 144, 97, 53, 88,
                                                                                220, 23, 117, 171, 179, 252, 92, 140,
                                                                                88, 110, 60, 141, 68, 125, 118, 121,
                                                                                34, 46, 19, 144, 51, 227, 130, 2,
                                                                                53, 227, 61, 165, 9, 30, 155, 11,
                                                                                184, 241, 161, 18, 207, 12, 143, 245});
CryptoPP::SecByteBlock g_privateKeyBytes = Utils::ByteVectorToSecBlock({100, 245, 118, 3, 181, 138, 241, 105,
                                                                        7, 193, 138, 134, 97, 35, 40, 110,
                                                                        28, 188, 232, 151, 144, 97, 53, 88,
                                                                        220, 23, 117, 171, 179, 252, 92, 140});

std::string g_privateKeyHex = "0x64f57603b58af16907c18a866123286e1cbce89790613558dc1775abb3fc5c8c";

CryptoPP::SecByteBlock g_privateKeySerializedOutput = Utils::ByteVectorToSecBlock({32, 100, 245, 118, 3, 181, 138, 241,
                                                                                   105, 7, 193, 138, 134, 97, 35, 40,
                                                                                   110, 28, 188, 232, 151, 144, 97, 53,
                                                                                   88, 220, 23, 117, 171, 179, 252, 92, 140});

CryptoPP::SecByteBlock g_publicKeyBytes = Utils::ByteVectorToSecBlock({88, 110, 60, 141, 68, 125, 118, 121,
                                                                       34, 46, 19, 144, 51, 227, 130, 2,
                                                                       53, 227, 61, 165, 9, 30, 155, 11,
                                                                       184, 241, 161, 18, 207, 12, 143, 245});
std::string g_publicKeyHex = "0x586e3c8d447d7679222e139033e3820235e33da5091e9b0bb8f1a112cf0c8ff5";

std::string g_accountAddress = "0x9f628c43d1c1c0f54683cf5ccbd2b944608df4ff2649841053b1790a4d7c187d";

CryptoPP::SecByteBlock g_publicKeySerializedOutput = Utils::ByteVectorToSecBlock({32, 88, 110, 60, 141, 68, 125, 118,
                                                                                  121, 34, 46, 19, 144, 51, 227, 130,
                                                                                  2, 53, 227, 61, 165, 9, 30, 155,
                                                                                  11, 184, 241, 161, 18, 207, 12, 143, 245});

CryptoPP::SecByteBlock g_privateKeyBytesInvalid = Utils::ByteVectorToSecBlock({
    100,
    245,
    118,
    3,
    181,
    138,
    241,
    105,
    7,
    193,
    138,
    134,
    97,
    35,
    40,
    110,
    28,
    188,
    232,
    151,
    144,
    97,
    53,
    88,
    220,
    23,
    117,
    171,
    179,
    252,
    92,
    140,
    88,
    110,
    60,
    141,
    68,
    125,
    118,
    121,
    34,
    46,
    19,
    144,
    51,
    227,
    130,
    2,
    53,
    227,
    61,
    165,
    9,
    30,
    155,
    11,
});

CryptoPP::SecByteBlock g_publicKeyBytesInvalid = Utils::ByteVectorToSecBlock({88, 110, 60, 141, 68, 125, 118, 121,
                                                                              34, 46, 19, 144, 51, 227, 130, 2,
                                                                              53, 227, 61, 165, 9, 30, 155, 11,
                                                                              184, 241, 161, 18, 207, 12, 143, 245});

std::string g_accountAddressHex = "0x9f628c43d1c1c0f54683cf5ccbd2b944608df4ff2649841053b1790a4d7c187d";
std::string g_ccountAuthKeyHex = "0x9f628c43d1c1c0f54683cf5ccbd2b944608df4ff2649841053b1790a4d7c187d";

CryptoPP::SecByteBlock g_messageUtf8Bytes = Utils::ByteVectorToSecBlock({87, 69, 76, 67, 79, 77, 69, 32,
                                                                         84, 79, 32, 65, 80, 84, 79, 83, 33});
std::string g_message = "WELCOME TO APTOS!";

CryptoPP::SecByteBlock g_signatureBytes = Utils::ByteVectorToSecBlock({170, 66, 187, 194, 169, 252, 117, 27,
                                                                       238, 238, 59, 49, 43, 132, 82, 196,
                                                                       69, 199, 212, 171, 134, 152, 3, 107,
                                                                       12, 249, 242, 228, 106, 9, 139, 176,
                                                                       44, 54, 159, 188, 141, 254, 253, 35,
                                                                       26, 18, 141, 138, 75, 185, 173, 207,
                                                                       228, 94, 7, 24, 139, 117, 140, 58,
                                                                       211, 152, 215, 248, 78, 130, 239, 5});

Ed25519Signature signatureObject(g_signatureBytes);

std::string g_signatureHex = "0xaa42bbc2a9fc751beeee3b312b8452c445c7d4ab8698036b0cf9f2e46a098bb02c369fbc8dfefd231a128d8a4bb9adcfe45e07188b758c3ad398d7f84e82ef05";

CryptoPP::SecByteBlock g_signatureSerializedOutput = Utils::ByteVectorToSecBlock({64, 170, 66, 187, 194, 169, 252, 117,
                                                                                  27, 238, 238, 59, 49, 43, 132, 82,
                                                                                  196, 69, 199, 212, 171, 134, 152, 3,
                                                                                  107, 12, 249, 242, 228, 106, 9, 139,
                                                                                  176, 44, 54, 159, 188, 141, 254, 253,
                                                                                  35, 26, 18, 141, 138, 75, 185, 173,
                                                                                  207, 228, 94, 7, 24, 139, 117, 140,
                                                                                  58, 211, 152, 215, 248, 78, 130, 239, 5});

TEST(AccountTest, GeneratePrivateKeysWithBytesSuccess)
{
    std::unique_ptr<PrivateKey> privateKey = std::make_unique<PrivateKey>(g_privateKeyBytes);
    ASSERT_TRUE(privateKey->KeyBytes().data() != nullptr);
    ASSERT_EQ(32, privateKey->KeyBytes().size());
    ASSERT_EQ(privateKey->KeyBytes(), g_privateKeyBytes);

    std::string privateKeyHex = privateKey->Key();
    ASSERT_EQ(privateKeyHex, g_privateKeyHex);
}

TEST(AccountTest, GenerateKeysWithBytesSuccess)
{
    std::unique_ptr<PrivateKey> privateKey = std::make_unique<PrivateKey>(g_privateKeyBytes);
    std::unique_ptr<PublicKey> publicKey = std::make_unique<PublicKey>(g_publicKeyBytes);

    ASSERT_TRUE(privateKey->KeyBytes().data() != nullptr);
    ASSERT_TRUE(publicKey->KeyBytes().data() != nullptr);

    std::string privateKeyHex = privateKey->ToString();
    std::string publicKeyHex = publicKey->ToString();

    ASSERT_EQ(privateKeyHex, g_privateKeyHex);
    ASSERT_EQ(publicKeyHex, g_publicKeyHex);
}

TEST(AccountTest, GenerateKeysWithStringSuccess)
{
    std::unique_ptr<PrivateKey> privateKey = std::make_unique<PrivateKey>(g_privateKeyHex);
    std::unique_ptr<PublicKey> publicKey = std::make_unique<PublicKey>(g_publicKeyHex);

    ASSERT_TRUE(privateKey->KeyBytes().data() != nullptr);
    ASSERT_TRUE(publicKey->KeyBytes().data() != nullptr);

    ASSERT_EQ(privateKey->KeyBytes(), g_privateKeyBytes);
    ASSERT_EQ(publicKey->KeyBytes(), g_publicKeyBytes);

    std::string privateKeyHex = privateKey->Key();
    std::string publicKeyHex = publicKey->Key();

    ASSERT_EQ(privateKeyHex, g_privateKeyHex);
    ASSERT_EQ(publicKeyHex, g_publicKeyHex);
}

TEST(AccountTest, GeneratePublicKeyFromPrivateKeySuccess)
{
    std::unique_ptr<PrivateKey> privateKey = std::make_unique<PrivateKey>(g_privateKeyHex);
    PublicKey publicKey = privateKey->GetPublicKey();

    ASSERT_EQ(g_publicKeyBytes, publicKey.KeyBytes());
}

TEST(AccountTest, PrivateKeyFromHexSignSuccess)
{
    std::unique_ptr<PrivateKey> privateKey = std::make_unique<PrivateKey>(g_privateKeyHex);
    ASSERT_EQ(privateKey->Key(), g_privateKeyHex);
    ASSERT_EQ(privateKey->KeyBytes(), g_privateKeyBytes);

    Ed25519Signature signature = privateKey->Sign(g_messageUtf8Bytes);
    ASSERT_EQ(signature, signatureObject);
}

TEST(AccountTest, PrivateKeyFromBytesSignSuccess)
{
    PrivateKey privateKey(g_privateKeyBytes);
    ASSERT_EQ(privateKey.Key(), g_privateKeyHex);
    ASSERT_EQ(privateKey.KeyBytes(), g_privateKeyBytes);

    Ed25519Signature signature = privateKey.Sign(g_messageUtf8Bytes);
    ASSERT_EQ(signature, signatureObject);
}

TEST(AccountTest, InvalidKeyGeneration)
{
    ASSERT_THROW({
        std::unique_ptr<PrivateKey> privateKey = std::make_unique<PrivateKey>(g_privateKeyBytesInvalid);
        std::unique_ptr<PublicKey> publicKey = std::make_unique<PublicKey>(g_publicKeyBytesInvalid);
    },
                 std::invalid_argument);
}

TEST(AccountTest, PublicKeySerialization)
{
    Serialization serializer;
    std::unique_ptr<PublicKey> publicKey = std::make_unique<PublicKey>(g_publicKeyBytes);
    publicKey->Serialize(serializer);
    CryptoPP::SecByteBlock output = Utils::ByteVectorToSecBlock(serializer.GetBytes());

    ASSERT_EQ(output, g_publicKeySerializedOutput);
}

TEST(AccountTest, PublicKeyDeserialization)
{
    Serialization serializer;
    PublicKey publicKey(g_publicKeyBytes);
    publicKey.Serialize(serializer);
    CryptoPP::SecByteBlock output = Utils::ByteVectorToSecBlock(serializer.GetBytes());

    ASSERT_EQ(output, g_publicKeySerializedOutput);

    Deserialization deserializer(Utils::SecBlockToByteVector(output));
    auto actualDeserialized = std::dynamic_pointer_cast<PublicKey>(PublicKey::Deserialize(deserializer));

    ASSERT_EQ(publicKey, *actualDeserialized);
}

TEST(AccountTest, PrivateKeySerialization)
{
    Serialization serializer;
    PrivateKey privateKey(g_privateKeyBytes);
    privateKey.Serialize(serializer);
    CryptoPP::SecByteBlock output = Utils::ByteVectorToSecBlock(serializer.GetBytes());

    ASSERT_EQ(output, g_privateKeySerializedOutput);
}

TEST(AccountTest, SignatureEquality)
{
    std::unique_ptr<Ed25519Signature> sigOne = std::make_unique<Ed25519Signature>(g_signatureBytes);
    std::unique_ptr<Ed25519Signature> sigTwo = std::make_unique<Ed25519Signature>(g_signatureBytes);
    ASSERT_TRUE(sigOne->Equals(*sigTwo));
}

TEST(AccountTest, SignatureSerialization)
{
    Serialization serializer;
    Ed25519Signature sig(g_signatureBytes);
    sig.Serialize(serializer);
    CryptoPP::SecByteBlock output = Utils::ByteVectorToSecBlock(serializer.GetBytes());
    ASSERT_EQ(output, g_signatureSerializedOutput);
}

TEST(AccountTest, SignatureDeserialization)
{
    Serialization serializer;
    Ed25519Signature sig(g_signatureBytes);
    sig.Serialize(serializer);
    CryptoPP::SecByteBlock output = Utils::ByteVectorToSecBlock(serializer.GetBytes());

    Deserialization deser(Utils::SecBlockToByteVector(output));
    auto actualSig = std::dynamic_pointer_cast<Ed25519Signature>(Ed25519Signature::Deserialize(deser));
    ASSERT_EQ(sig, *actualSig);
}

TEST(AccountTests, GenerateAccountAddressFromPublicKey)
{
    PublicKey publicKey(g_publicKeyBytes);
    AccountAddress accountAddress = AccountAddress::FromKey(publicKey.KeyBytes());
    ASSERT_EQ(accountAddress.ToString(), g_accountAddressHex);
}

TEST(AccountTests, CreateAccountFromKeys)
{
    Account acc(g_privateKeyBytes, g_publicKeyBytes);
    auto privateKey = acc.getPrivateKey();
    auto publicKey = acc.getPublicKey();

    PublicKey validPublicKey(g_publicKeyBytes);
    PrivateKey validPrivateKey(g_privateKeyBytes);

    ASSERT_TRUE(privateKey->Key() == validPrivateKey.Key());
    ASSERT_TRUE(publicKey->Key() == validPublicKey.Key());
}

TEST(AccountTests, GenerateAccountFromPrivateKeyStringSuccess)
{
    Account acc = Account::LoadKey(g_privateKeyHex);
    auto privateKey = acc.getPrivateKey();
    auto publicKey = acc.getPublicKey();

    PublicKey expectedPublicKey(g_publicKeyBytes);
    PrivateKey expectedPrivateKey(g_privateKeyBytes);

    ASSERT_TRUE(privateKey->Key() == expectedPrivateKey.Key());
    ASSERT_TRUE(publicKey->Key() == expectedPublicKey.Key());

    ASSERT_EQ(g_accountAddress, acc.getAccountAddress()->ToString());
}
TEST(AccountTests, CreateDefaultAccountSuccess)
{
    Account acc = Account();
    ASSERT_TRUE(acc.getPrivateKey() != nullptr);
    ASSERT_TRUE(acc.getPublicKey() != nullptr);
    ASSERT_TRUE(acc.getAccountAddress() != nullptr);
}

TEST(AccountTests, InvalidAccountCreationWithShorterKeys)
{
    ASSERT_THROW(Account acc(g_privateKeyBytesInvalid, g_publicKeyBytesInvalid), std::invalid_argument);
}

TEST(AccountTests, AuthKeyGeneration)
{
    Account acc = Account(g_privateKeyBytes, g_publicKeyBytes);
    std::string authKey = acc.AuthKey();
    ASSERT_EQ(authKey, g_ccountAuthKeyHex);
}

TEST(AccountTests, AccountSignSuccess)
{
    Account acc = Account(g_privateKeyBytes, g_publicKeyBytes);
    Ed25519Signature signature = acc.Sign(g_messageUtf8Bytes);
    ASSERT_EQ(signature, signatureObject);
}

TEST(AccountTests, AccountSignVerify)
{
    Account acc = Account(g_privateKeyBytes, g_publicKeyBytes);
    Ed25519Signature signature = acc.Sign(g_messageUtf8Bytes);
    ASSERT_EQ(signature, signatureObject);
    bool verify = acc.Verify(g_messageUtf8Bytes, signature);
    ASSERT_TRUE(verify);
}

TEST(AccountTests, TestMultisig)
{
    // Generate signatory private keys.
    PrivateKey privateKey1 = PrivateKey::FromHex(
        "4e5e3be60f4bbd5e98d086d932f3ce779ff4b58da99bf9e5241ae1212a29e5fe");
    PrivateKey privateKey2 = PrivateKey::FromHex(
        "1e70e49b78f976644e2c51754a2f049d3ff041869c669523ba95b172c7329901");

    // Generate multisig public key with threshold of 1.
    std::vector<PublicKey> publicKeys;
    publicKeys.push_back(privateKey1.GetPublicKey());
    publicKeys.push_back(privateKey2.GetPublicKey());
    MultiPublicKey multiSigPublicKey = MultiPublicKey(publicKeys, 1);

    // Get public key BCS representation.
    Serialization serializer;
    multiSigPublicKey.Serialize(serializer);
    std::string publicKeyBcs = Utils::HexStringFromByteArray(serializer.GetBytes());
    // Check against expected BCS representation.
    std::string expectedPublicKeyBcs = "41754bb6a4720a658bdd5f532995955db0971ad3519acbde2f1149c3857348006c"
                                       "1634cd4607073f2be4a6f2aadc2b866ddb117398a675f2096ed906b20e0bf2c901";

    ASSERT_EQ(publicKeyBcs, expectedPublicKeyBcs);

    // Get public key bytes representation.
    std::vector<uint8_t> publicKeyBytes = multiSigPublicKey.ToBytes();

    // Convert back to multisig class instance from bytes.
    MultiPublicKey multisigPublicKey = MultiPublicKey::FromBytes(publicKeyBytes);

    // Get public key BCS representation.
    serializer = Serialization();
    multisigPublicKey.Serialize(serializer);
    std::string publicKeyBCs = Utils::HexStringFromByteArray(serializer.GetBytes());

    // Assert BCS representation is the same.
    ASSERT_EQ(publicKeyBcs, publicKeyBCs);

    // Have one signer sign arbitrary message.
    Ed25519Signature signature = privateKey2.Sign(Utils::StringToSecByteBlock("multisig"));

    // Compose multisig signature.
    std::vector<std::pair<PublicKey, Ed25519Signature>> signMap = {
        std::make_pair(privateKey2.GetPublicKey(), signature)};
    MultiSignature multiSignature = MultiSignature(multisigPublicKey, signMap);

    // Get signature BCS representation.
    serializer = Serialization();
    multiSignature.Serialize(serializer);
    std::vector<uint8_t> multisigBcsBytes = serializer.GetBytes();
    std::string multisigSignatureBcs = Utils::HexStringFromByteArray(serializer.GetBytes());

    // Check against expected BCS representation.
    std::string expectedMultisigSignatureBcs = "4402e90d8f300d79963cb7159ffa6f620f5bba4af5d32a7176bfb5480b43897cf4886bbb4042182f4647c9b04f02dbf989966f0facceec52d22bdcc7ce631bfc0c40000000";

    ASSERT_EQ(multisigSignatureBcs, expectedMultisigSignatureBcs);
}

TEST(AccountTests, TestMultiEd25519)
{
    PrivateKey privateKey1 = PrivateKey::FromHex(
        "4e5e3be60f4bbd5e98d086d932f3ce779ff4b58da99bf9e5241ae1212a29e5fe");

    PrivateKey privateKey2 = PrivateKey::FromHex(
        "1e70e49b78f976644e2c51754a2f049d3ff041869c669523ba95b172c7329901");

    MultiPublicKey multiSigPublicKey = MultiPublicKey(std::vector<PublicKey>{privateKey1.GetPublicKey(), privateKey2.GetPublicKey()}, 1);

    AccountAddress expected = AccountAddress::FromHex("835bb8c5ee481062946b18bbb3b42a40b998d6bf5316ca63834c959dc739acf0");

    AccountAddress actual = AccountAddress::FromMultiEd25519(multiSigPublicKey);

    ASSERT_EQ(expected, actual);
}

TEST(AccountTests, TestResourceAccount)
{
    AccountAddress baseAddress = AccountAddress::FromHex("b0b");
    AccountAddress expected = AccountAddress::FromHex("ee89f8c763c27f9d942d496c1a0dcf32d5eacfe78416f9486b8db66155b163b0");

    std::vector<uint8_t> seed = {0x0b, 0x00, 0x0b};
    AccountAddress actual = AccountAddress::ForResourceAccount(baseAddress, Utils::ByteVectorToSecBlock(seed));

    ASSERT_EQ(actual, expected);
}

TEST(AccountTests, TestNamedObject)
{
    AccountAddress baseAddress = AccountAddress::FromHex("b0b");
    AccountAddress expected = AccountAddress::FromHex("f417184602a828a3819edf5e36285ebef5e4db1ba36270be580d6fd2d7bcc321");

    CryptoPP::SecByteBlock seed = Utils::ByteVectorToSecBlock(
        {'b', 'o', 'b', '\'', 's', ' ', 'c', 'o', 'l', 'l', 'e', 'c', 't', 'i', 'o', 'n'});
    AccountAddress actual = AccountAddress::ForNamedObject(baseAddress, seed);

    ASSERT_EQ(actual, expected);
}

TEST(AccountTests, TestCollection)
{
    AccountAddress baseAddress = AccountAddress::FromHex("b0b");
    AccountAddress expected = AccountAddress::FromHex("f417184602a828a3819edf5e36285ebef5e4db1ba36270be580d6fd2d7bcc321");

    AccountAddress actual = AccountAddress::ForNamedCollection(baseAddress, "bob's collection");

    ASSERT_EQ(actual, expected);
}

TEST(AccountTests, TestToken)
{
    AccountAddress baseAddress = AccountAddress::FromHex("b0b");
    AccountAddress expected = AccountAddress::FromHex("e20d1f22a5400ba7be0f515b7cbd00edc42dbcc31acc01e31128b2b5ddb3c56e");

    AccountAddress actual = AccountAddress::ForNamedToken(baseAddress, "bob's collection", "bob's token");

    ASSERT_EQ(actual, expected);
}

TEST(AccountTests, TestToStandardString)
{
    // Test special address: 0x0
    ASSERT_EQ(AccountAddress::FromHex("0x0000000000000000000000000000000000000000000000000000000000000000").ToString(), "0x0");

    // Test special address: 0x1
    ASSERT_EQ(AccountAddress::FromHex("0x0000000000000000000000000000000000000000000000000000000000000001").ToString(), "0x1");

    // Test special address: 0x4
    ASSERT_EQ(AccountAddress::FromHex("0x0000000000000000000000000000000000000000000000000000000000000004").ToString(), "0x4");

    // Test special address: 0xF
    ASSERT_EQ(AccountAddress::FromHex("0x000000000000000000000000000000000000000000000000000000000000000f").ToString(), "0xf");

    // Test special address from short no 0x: d
    ASSERT_EQ(AccountAddress::FromHex("d").ToString(), "0xd");

    // Test non-special address from long:
    // 0x0000000000000000000000000000000000000000000000000000000000000010
    ASSERT_EQ(AccountAddress::FromHex("0x0000000000000000000000000000000000000000000000000000000000000010").ToString(), "0x0000000000000000000000000000000000000000000000000000000000000010");

    // Test non-special address from long:
    // 0x000000000000000000000000000000000000000000000000000000000000001f
    ASSERT_EQ(AccountAddress::FromHex("0x000000000000000000000000000000000000000000000000000000000000001f").ToString(), "0x000000000000000000000000000000000000000000000000000000000000001f");

    // Test non-special address from long:
    // 0x00000000000000000000000000000000000000000000000000000000000000a0
    ASSERT_EQ(AccountAddress::FromHex("0x00000000000000000000000000000000000000000000000000000000000000a0").ToString(), "0x00000000000000000000000000000000000000000000000000000000000000a0");

    // Test non-special address from long no 0x:
    // ca843279e3427144cead5e4d5999a3d0ca843279e3427144cead5e4d5999a3d0
    ASSERT_EQ(AccountAddress::FromHex("ca843279e3427144cead5e4d5999a3d0ca843279e3427144cead5e4d5999a3d0").ToString(), "0xca843279e3427144cead5e4d5999a3d0ca843279e3427144cead5e4d5999a3d0");

    // Test non-special address from long no 0x:
    // 1000000000000000000000000000000000000000000000000000000000000000
    ASSERT_EQ(AccountAddress::FromHex("1000000000000000000000000000000000000000000000000000000000000000").ToString(), "0x1000000000000000000000000000000000000000000000000000000000000000");

    // Demonstrate that neither leading nor trailing zeroes get trimmed for
    // non-special addresses:
    // 0f00000000000000000000000000000000000000000000000000000000000000
    ASSERT_EQ(AccountAddress::FromHex("0f00000000000000000000000000000000000000000000000000000000000000").ToString(), "0x0f00000000000000000000000000000000000000000000000000000000000000");
}

TEST(AccountAddressTests, GetHashCodeConsistency)
{
    // Create two AccountAddress instances with the same value
    AccountAddress address1 = AccountAddress::FromHex("123abc");
    AccountAddress address2 = AccountAddress::FromHex("123abc");

    // Get hash codes
    size_t hash1 = address1.GetHashCode();
    size_t hash2 = address2.GetHashCode();

    // Test if the same address returns the same hash code
    ASSERT_EQ(hash1, hash2);
}

TEST(AccountAddressTests, VariantMethod)
{
    // Setup: Create an AccountAddress instance
    AccountAddress address = AccountAddress::FromHex("123abc"); // Example initialization

    // Execute: Call the Variant method
    TypeTag variantType = address.Variant();

    // Verify: Check if the returned TypeTag is ACCOUNT_ADDRESS
    ASSERT_EQ(variantType, TypeTag::ACCOUNT_ADDRESS);
}

TEST(AuthenticationKeyTest, ConstructorWithInvalidLength)
{
    // Setup: Create a SecByteBlock with the incorrect size
    size_t incorrectSize = AuthenticationKey::LENGTH + 1; // or any other incorrect size
    CryptoPP::SecByteBlock bytes(incorrectSize);

    // Execute & Verify: Ensure the constructor throws a std::invalid_argument exception
    ASSERT_THROW(AuthenticationKey authKey(bytes), std::invalid_argument);
}

TEST(AuthenticationKeyTest, FromMultiEd25519PublicKey)
{
    // Setup: Create a vector of PublicKeys and define a valid threshold
    PublicKey key1(g_publicKeyBytes);
    PublicKey key2(g_publicKeyHex);
    std::vector<PublicKey> publicKeys = {key1, key2};
    int validThreshold = 15;

    // Execute: Construct the MultiEd25519PublicKey
    Aptos::Accounts::Types::MultiEd25519PublicKey multiPublicKey(publicKeys, validThreshold);

    AuthenticationKey key = AuthenticationKey::FromMultiEd25519PublicKey(multiPublicKey);

    ASSERT_FALSE(key.DerivedAddress().empty());
}

TEST(PrivateKeyTest, ConstructorWithKeyInvalid)
{
    // Setup: Create a PrivateKey instance
    std::string keyString = "4e5e3be60f4bbd5e98d086d932f3ce779ff4b58da99bf9e5241ae1212a29e5feaa";

    ASSERT_THROW(PrivateKey privateKey(keyString), std::invalid_argument);
}

TEST(PrivateKeyTest, ConstructorWithKeyEmpty)
{
    // Setup: Create a PrivateKey instance
    std::string keyString = "";

    ASSERT_THROW(PrivateKey privateKey(keyString), std::invalid_argument);
}

TEST(PrivateKeyTest, RandomMethod)
{
    // Setup: Create a PrivateKey instance
    std::string keyString = "4e5e3be60f4bbd5e98d086d932f3ce779ff4b58da99bf9e5241ae1212a29e5fe";
    PrivateKey privateKey(keyString);

    PrivateKey randomPrivateKey = privateKey.Random();
    // Verify: Check if the serialized data matches the expected output
    ASSERT_TRUE(privateKey != randomPrivateKey);
}

TEST(PrivateKeyTest, SerializeMethodWithEmpty)
{
    // Setup: Create a PrivateKey instance
    std::string keyString = "4e5e3be60f4bbd5e98d086d932f3ce779ff4b58da99bf9e5241ae1212a29e5fe";
    PrivateKey privateKey(keyString);

    Serialization serializer;

    // Execute: Call the Serialize method
    privateKey.Serialize(serializer);

    // Retrieve the serialized data
    std::vector<uint8_t> serializedData = serializer.GetBytes();

    // Verify: Check if the serialized data matches the expected output
    ASSERT_TRUE(serializedData.size() > 0);
}

TEST(PrivateKeyTest, EqualsWithDifferentKeys)
{
    // Setup: Create two PrivateKey instances with different keys
    PrivateKey key1 = PrivateKey::FromHex(
        "4e5e3be60f4bbd5e98d086d932f3ce779ff4b58da99bf9e5241ae1212a29e5fe");
    PrivateKey key2 = PrivateKey::FromHex(
        "1e70e49b78f976644e2c51754a2f049d3ff041869c669523ba95b172c7329901");

    // Execute & Verify: Check if Equals returns false for different keys
    ASSERT_FALSE(key1 == key2);
}

TEST(PrivateKeyTest, SetKey)
{
    // Setup: Create a PrivateKey instance
    PrivateKey privateKey = PrivateKey::FromHex(
        "4e5e3be60f4bbd5e98d086d932f3ce779ff4b58da99bf9e5241ae1212a29e5fe");

    // The key to set
    std::string testKey = "1e70e49b78f976644e2c51754a2f049d3ff041869c669523ba95b172c7329901";

    // Execute: Set the key
    privateKey.Key(testKey);

    // Verify: Check if the key was correctly set
    // This assumes there's a getter for the key. If not, this part needs to be adjusted.
    ASSERT_EQ(privateKey.Key(), testKey);
}

TEST(PrivateKeyTest, KeyBytesWithInvalidSize)
{
    // Setup: Create a PrivateKey instance and a SecByteBlock of incorrect size
    PrivateKey privateKey = PrivateKey::FromHex(
        "4e5e3be60f4bbd5e98d086d932f3ce779ff4b58da99bf9e5241ae1212a29e5fe");
    size_t incorrectSize = PrivateKey::KeyLength + 1; // Assuming incorrect size
    CryptoPP::SecByteBlock invalidSizeBlock(incorrectSize);

    // Execute & Verify: Check if setting a SecByteBlock of incorrect size throws std::invalid_argument
    ASSERT_THROW(privateKey.KeyBytes(invalidSizeBlock), std::invalid_argument);
}

TEST(PrivateKeyTest, KeyBytesWithValidValue)
{
    // Setup: Create a PrivateKey instance and a valid SecByteBlock
    PrivateKey privateKey = PrivateKey::FromHex(
        "4e5e3be60f4bbd5e98d086d932f3ce779ff4b58da99bf9e5241ae1212a29e5fe");
    CryptoPP::SecByteBlock validBlock(PrivateKey::KeyLength);
    // Optionally fill validBlock with some data

    // Execute: Set the valid SecByteBlock
    ASSERT_NO_THROW(privateKey.KeyBytes(validBlock));

    // Verify: Check if _keyBytes was set correctly
    // This assumes you have a way to get _keyBytes for verification
    ASSERT_EQ(privateKey.KeyBytes(), validBlock);
}

TEST(PrivateKeyTest, GetHashCode)
{
    // Setup: Create a PrivateKey instance and a valid SecByteBlock
    PrivateKey privateKey = PrivateKey::FromHex(
        "4e5e3be60f4bbd5e98d086d932f3ce779ff4b58da99bf9e5241ae1212a29e5fe");
    CryptoPP::SecByteBlock validBlock(PrivateKey::KeyLength);
    // Optionally fill validBlock with some data

    // Execute: Set the valid SecByteBlock
    ASSERT_NO_THROW(privateKey.KeyBytes(validBlock));

    ASSERT_TRUE(privateKey.GetHashCode() != 0);
}

TEST(PrivateKeyTest, SignEmptyMessage)
{
    // Setup: Create a PrivateKey instance and a valid SecByteBlock
    PrivateKey privateKey = PrivateKey::FromHex(
        "4e5e3be60f4bbd5e98d086d932f3ce779ff4b58da99bf9e5241ae1212a29e5fe");
    CryptoPP::SecByteBlock validBlock(PrivateKey::KeyLength);
    // Optionally fill validBlock with some data
    ASSERT_NO_THROW(privateKey.Sign(Utils::StringToSecByteBlock("multisig")));
}

TEST(PublicKeyTest, IsOnCurveAlwaysReturnsFalse)
{
    // Setup: Create a PublicKey instance with any key, since the method isn't implemented yet
    // Replace with an arbitrary key; it doesn't matter for this version of the test
    std::string arbitraryKey = "0x586e3c8d447d7679222e139033e3820235e33da5091e9b0bb8f1a112cf0c8ff5";
    PublicKey publicKey(arbitraryKey);

    // Execute & Verify: Check if IsOnCurve currently always returns false
    ASSERT_FALSE(publicKey.IsOnCurve());
}

TEST(PublicKeyTest, GetHashCodeConsistency)
{
    // Setup: Create a PublicKey instance
    std::string keyString = "0x586e3c8d447d7679222e139033e3820235e33da5091e9b0bb8f1a112cf0c8ff5";
    PublicKey publicKey(keyString);

    // Execute: Get the hash code twice
    size_t hashCode1 = publicKey.GetHashCode();
    size_t hashCode2 = publicKey.GetHashCode();

    // Verify: Check if the hash codes are consistent
    ASSERT_EQ(hashCode1, hashCode2);
}

TEST(PublicKeyTest, GetHashCodeDifferentKeys)
{
    // Setup: Create two PublicKey instances with different keys
    PublicKey key1("0x586e3c8d447d7679222e139033e3820235e33da5091e9b0bb8f1a112cf0c8ff5");
    PublicKey key2("0x9f628c43d1c1c0f54683cf5ccbd2b944608df4ff2649841053b1790a4d7c187d");

    // Execute: Get the hash codes for each key
    size_t hashCode1 = key1.GetHashCode();
    size_t hashCode2 = key2.GetHashCode();

    // Verify: Ideally, the hash codes should be different
    // Note: This is not a strict requirement as hash collisions can happen
    ASSERT_NE(hashCode1, hashCode2);
}

TEST(PublicKeyTest, ConstructorWithEmptyValue)
{
    // Setup: Create an empty SecByteBlock
    CryptoPP::SecByteBlock emptySecByteBlock;

    // Execute & Verify: Check if constructing with an empty SecByteBlock throws std::invalid_argument
    ASSERT_THROW({
        std::unique_ptr<PublicKey> publicKey = std::make_unique<PublicKey>(emptySecByteBlock);
    },
                 std::invalid_argument);
}

TEST(PublicKeyTest, ConstructorWithInvalidKey)
{
    // Example of an invalid key string (e.g., too short)
    std::string invalidKey = "short_key";

    // Execute & Verify: Check if constructing with an invalid key string throws std::invalid_argument
    ASSERT_THROW({
        std::unique_ptr<PublicKey> publicKey = std::make_unique<PublicKey>(g_publicKeySerializedOutput);
    },
                 std::invalid_argument);
}

TEST(PublicKeyTest, ConstructorWithValidKey)
{
    // Example of a valid key string
    std::string validKey = "0x586e3c8d447d7679222e139033e3820235e33da5091e9b0bb8f1a112cf0c8ff5"; // Replace with an actual valid key

    // Execute & Verify: Check if constructing with a valid key does not throw
    ASSERT_NO_THROW(std::unique_ptr<PublicKey> publicKey = std::make_unique<PublicKey>(g_publicKeyHex));

    // Additional verification steps can be added here if needed
}

TEST(PublicKeyTest, ConstructorWithEmptyKey)
{
    // Setup: Define an empty key
    std::string emptyKey;

    // Execute & Verify: Ensure the constructor throws an std::invalid_argument exception
    // when an empty key is passed
    ASSERT_THROW({
        std::unique_ptr<PublicKey> publicKey = std::make_unique<PublicKey>(emptyKey);
    },
                 std::invalid_argument);
}

// setKey PublicKey
TEST(PublicKeyTest, SetKeyWithEmptyString)
{
    // Setup: Create a PublicKey instance and an empty key string
    PublicKey publicKey(g_publicKeyHex);
    std::string emptyKey = "";

    // Execute & Verify: Ensure the method throws an std::invalid_argument exception
    ASSERT_THROW(publicKey.setKey(emptyKey), std::invalid_argument);
}

TEST(PublicKeyTest, SetKeyWithInvalidFormat)
{
    // Setup: Create a PublicKey instance and an invalid key string
    PublicKey publicKey(g_publicKeyHex);
    std::string invalidKey = "0x586e3c8d447d7679222e1aa39033e3820235e33da5091e9b0bb8f1a112cf0c8ff5";

    // Execute & Verify: Ensure the method throws an std::invalid_argument exception
    ASSERT_THROW(publicKey.setKey(invalidKey), std::invalid_argument);
}

TEST(PublicKeyTest, SetKeySuccessfully)
{
    // Setup: Create a PublicKey instance and a valid key string
    PublicKey publicKey(g_publicKeyHex);
    std::string validKey = "0x586e3c8d447d7679222e139033e3820235e33da5091e9b0bb8f1a112cf0c8ff5"; // Replace with a valid hex key format

    // Execute: Set the key
    ASSERT_NO_THROW(publicKey.setKey(validKey));

    // Verify: Check if the key was set correctly
    // This part of the test depends on the available public methods of PublicKey.
    // For example, if there's a method to get a property that changes when the key is set:
    // ASSERT_EQ(publicKey.getSomeProperty(), expectedPropertyValue);
}

// setKeyBytes PublicKey.cpp
TEST(PublicKeyTest, SetKeyBytesWithEmptyBlock)
{
    // Setup: Create a PublicKey instance and an empty SecByteBlock
    PublicKey publicKey(g_publicKeyBytes);
    CryptoPP::SecByteBlock emptyBlock;

    // Execute & Verify: Ensure the method throws an std::invalid_argument exception
    ASSERT_THROW(publicKey.setKeyBytes(emptyBlock), std::invalid_argument);
}

TEST(PublicKeyTest, SetKeyBytesWithIncorrectLength)
{
    // Setup: Create a PublicKey instance and a SecByteBlock of incorrect length
    PublicKey publicKey(g_publicKeyBytes);
    size_t incorrectLength = PublicKey::KeyLength - 1; // Assuming KeyLength is accessible
    CryptoPP::SecByteBlock blockWithIncorrectLength(incorrectLength);

    // Execute & Verify: Ensure the method throws an std::invalid_argument exception
    ASSERT_THROW(publicKey.setKeyBytes(blockWithIncorrectLength), std::invalid_argument);
}

TEST(PublicKeyTest, SetKeyBytesSuccessfully)
{
    // Setup: Create a PublicKey instance and a valid SecByteBlock
    PublicKey publicKey(g_publicKeyBytes);
    CryptoPP::SecByteBlock validBlock(PublicKey::KeyLength); // Assuming KeyLength is accessible

    // Execute: Set the key bytes
    ASSERT_NO_THROW(publicKey.setKeyBytes(validBlock));

    // Verify: Check if the key bytes were set correctly
    // This part of the test depends on the available public methods of PublicKey.
    // For example, if there's a method to get a property or the key bytes themselves:
    // ASSERT_EQ(publicKey.getSomeProperty(), expectedPropertyValue);
    // or
    // ASSERT_EQ(publicKey.getKeyBytes(), validBlock);
}

// MultiEd25519PublicKey MultiEd25519PublicKey.cpp
TEST(MultiEd25519PublicKeyTest, ConstructorThresholdExceedsLimit)
{
    // Setup: Create a vector of PublicKeys and define a threshold that exceeds the limit
    PublicKey key1(g_publicKeyBytes);
    PublicKey key2(g_publicKeyHex);
    std::vector<PublicKey> publicKeys = {key1, key2};
    int invalidThreshold = Aptos::Accounts::Types::MultiEd25519PublicKey::MAX_SIGNATURES_SUPPORTED + 1;

    // Execute & Verify: Ensure the constructor throws an std::invalid_argument exception
    ASSERT_THROW(Aptos::Accounts::Types::MultiEd25519PublicKey(publicKeys, invalidThreshold), std::invalid_argument);
}

TEST(MultiEd25519PublicKeyTest, ConstructorSuccessfulInitialization)
{
    // Setup: Create a vector of PublicKeys and define a valid threshold
    PublicKey key1(g_publicKeyBytes);
    PublicKey key2(g_publicKeyHex);
    std::vector<PublicKey> publicKeys = {key1, key2};
    int validThreshold = 15;

    // Execute: Construct the MultiEd25519PublicKey
    Aptos::Accounts::Types::MultiEd25519PublicKey multiPublicKey(publicKeys, validThreshold);

    // Verify: Check if the object is initialized correctly
    // This part of the test depends on the available public methods of MultiEd25519PublicKey.
    // For example:
    ASSERT_TRUE(multiPublicKey.PublicKeys.size() > 0);
}

TEST(MultiPublicKeyTest, ConstructorFail)
{
    // Setup: Create a vector of PublicKeys and define a valid threshold
    PublicKey key1(g_publicKeyBytes);
    PublicKey key2(g_publicKeyHex);
    std::vector<PublicKey> publicKeys = {};
    int validThreshold = 15;
    std::vector<uint8_t> keyBytes;

    ASSERT_THROW(Aptos::Accounts::MultiPublicKey(publicKeys, validThreshold), std::invalid_argument);
    ASSERT_THROW(Aptos::Accounts::MultiPublicKey::FromBytes(keyBytes), std::invalid_argument);
}

TEST(MultiPublicKeyTest, NoCheck)
{
    // Setup: Create a vector of PublicKeys and define a valid threshold
    PublicKey key1(g_publicKeyBytes);
    PublicKey key2(g_publicKeyHex);
    std::vector<PublicKey> publicKeys = {};
    int validThreshold = 15;
    std::vector<uint8_t> keyBytes;

    ASSERT_NO_THROW(Aptos::Accounts::MultiPublicKey(publicKeys, validThreshold, false));
}

TEST(MultiPublicKeyTest, ConstructorFailThressholdMin)
{
    // Setup: Create a vector of PublicKeys and define a valid threshold
    PublicKey key1(g_publicKeyBytes);
    PublicKey key2(g_publicKeyHex);
    std::vector<PublicKey> publicKeys = {key1, key2};
    int validThreshold = 0;
    std::vector<uint8_t> keyBytes({88, 110, 60, 141, 68, 125, 118, 121,
                                   34, 46, 19, 144, 51, 227, 130, 2,
                                   53, 227, 61, 165, 9, 30, 155, 11,
                                   184, 241, 161, 18, 207, 12, 143, 245});
    keyBytes.push_back((uint8_t)0);

    ASSERT_THROW(Aptos::Accounts::MultiPublicKey(publicKeys, validThreshold), std::invalid_argument);
    ASSERT_THROW(Aptos::Accounts::MultiPublicKey::FromBytes(keyBytes), std::invalid_argument);
}

TEST(MultiPublicKeyTest, ToString)
{
    // Setup: Create a vector of PublicKeys and define a valid threshold
    PublicKey key1(g_publicKeyBytes);
    PublicKey key2(g_publicKeyHex);
    std::vector<PublicKey> publicKeys = {key1, key2};
    int validThreshold = 1;

    Aptos::Accounts::MultiPublicKey multiPublicKey(publicKeys, validThreshold);

    ASSERT_EQ(multiPublicKey.ToString(), "1-of-2 Multi-Ed25519 public key");
}

TEST(Ed25519Bip32Test, IsValidPath)
{
    bool result = Ed25519Bip32::IsValidPath("m/44'/0'/0'");
    ASSERT_TRUE(result);
}

TEST(Ed25519Bip32Test, DerivePath)
{
    std::vector<uint8_t> seed;
    Ed25519Bip32 ed25519Bip32(seed);
    ASSERT_THROW(ed25519Bip32.DerivePath(""), std::invalid_argument);
}

TEST(Ed25519Bip32Test, InValidPath)
{

    // Test with a path that doesn't start with 'm'
    EXPECT_FALSE(Ed25519Bip32::IsValidPath("44'/0'/0'"));

    // Test with a path that doesn't have any slashes after 'm'
    EXPECT_FALSE(Ed25519Bip32::IsValidPath("m"));

    // Test with a path that has a non-numeric path component
    EXPECT_FALSE(Ed25519Bip32::IsValidPath("m/44'/0'/a'"));

    // Test with an empty path
    EXPECT_FALSE(Ed25519Bip32::IsValidPath(""));

    // Test with a path that has a part with no digits
    EXPECT_FALSE(Ed25519Bip32::IsValidPath("m/'/0'/0'"));
}

TEST(PrivateKey, keyEmpty)
{
    PrivateKey privateKey(g_privateKeyHex);
    CryptoPP::SecByteBlock value;
    EXPECT_ANY_THROW(privateKey.KeyBytes(value));
    EXPECT_ANY_THROW(PrivateKey privateKey2(value));
}