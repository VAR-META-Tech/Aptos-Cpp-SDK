//
// Created by Anh NPH on 27/09/2023.
//

#include "gtest/gtest.h"
#include <string>
#include "../Accounts/Signature.h"
#include "../Accounts/PublicKey.h"
#include "../Accounts/PrivateKey.h"
#include "../Accounts/AccountAddress.h"
#include "../BCS/Serialization.h"
#include "../Accounts/multipublickey.h"
#include "../HDWallet/Utils/Utils.h"
#include "../Accounts/multisignature.h"
#include "../Accounts/Account.h"

// Extended PrivateKey for reference
CryptoPP::SecByteBlock g_extendedPrivateKeyBytes = Utils::ByteVectorToSecBlock({
    100, 245, 118, 3, 181, 138, 241, 105,
    7, 193, 138, 134, 97, 35, 40, 110,
    28, 188, 232, 151, 144, 97, 53, 88,
    220, 23, 117, 171, 179, 252, 92, 140,
    88, 110, 60, 141, 68, 125, 118, 121,
    34, 46, 19, 144, 51, 227, 130, 2,
    53, 227, 61, 165, 9, 30, 155, 11,
    184, 241, 161, 18, 207, 12, 143, 245
});
CryptoPP::SecByteBlock g_privateKeyBytes = Utils::ByteVectorToSecBlock({
    100, 245, 118, 3, 181, 138, 241, 105,
    7, 193, 138, 134, 97, 35, 40, 110,
    28, 188, 232, 151, 144, 97, 53, 88,
    220, 23, 117, 171, 179, 252, 92, 140
});

std::string g_privateKeyHex = "0x64f57603b58af16907c18a866123286e1cbce89790613558dc1775abb3fc5c8c";

CryptoPP::SecByteBlock g_privateKeySerializedOutput = Utils::ByteVectorToSecBlock({
    32, 100, 245, 118, 3, 181, 138, 241,
    105, 7, 193, 138, 134, 97, 35, 40,
    110, 28, 188, 232, 151, 144, 97, 53,
    88, 220, 23, 117, 171, 179, 252, 92, 140
});

CryptoPP::SecByteBlock g_publicKeyBytes = Utils::ByteVectorToSecBlock({
    88, 110, 60, 141, 68, 125, 118, 121,
    34, 46, 19, 144, 51, 227, 130, 2,
    53, 227, 61, 165, 9, 30, 155, 11,
    184, 241, 161, 18, 207, 12, 143, 245
});
std::string g_publicKeyHex = "0x586e3c8d447d7679222e139033e3820235e33da5091e9b0bb8f1a112cf0c8ff5";

std::string g_accountAddress = "0x9f628c43d1c1c0f54683cf5ccbd2b944608df4ff2649841053b1790a4d7c187d";

CryptoPP::SecByteBlock g_publicKeySerializedOutput = Utils::ByteVectorToSecBlock({
    32, 88, 110, 60, 141, 68, 125, 118,
    121, 34, 46, 19, 144, 51, 227, 130,
    2, 53, 227, 61, 165, 9, 30, 155,
    11, 184, 241, 161, 18, 207, 12, 143, 245
});

CryptoPP::SecByteBlock g_privateKeyBytesInvalid = Utils::ByteVectorToSecBlock({
    100, 245, 118, 3, 181, 138, 241, 105,
    7, 193, 138, 134, 97, 35, 40, 110,
    28, 188, 232, 151, 144, 97, 53, 88,
    220, 23, 117, 171, 179, 252, 92, 140,
    88, 110, 60, 141, 68, 125, 118, 121,
    34, 46, 19, 144, 51, 227, 130, 2,
    53, 227, 61, 165, 9, 30, 155, 11,
});

CryptoPP::SecByteBlock g_publicKeyBytesInvalid = Utils::ByteVectorToSecBlock({
    88, 110, 60, 141, 68, 125, 118, 121,
    34, 46, 19, 144, 51, 227, 130, 2,
    53, 227, 61, 165, 9, 30, 155, 11,
    184, 241, 161, 18, 207, 12, 143, 245
});

std::string g_accountAddressHex = "0x9f628c43d1c1c0f54683cf5ccbd2b944608df4ff2649841053b1790a4d7c187d";
std::string g_ccountAuthKeyHex = "0x9f628c43d1c1c0f54683cf5ccbd2b944608df4ff2649841053b1790a4d7c187d";

CryptoPP::SecByteBlock g_messageUtf8Bytes = Utils::ByteVectorToSecBlock({
                                                                         87, 69, 76, 67, 79, 77, 69, 32,
                                                                         84, 79, 32, 65, 80, 84, 79, 83, 33 });
std::string g_message = "WELCOME TO APTOS!";

CryptoPP::SecByteBlock g_signatureBytes = Utils::ByteVectorToSecBlock({
    170, 66, 187, 194, 169, 252, 117, 27,
    238, 238, 59, 49, 43, 132, 82, 196,
    69, 199, 212, 171, 134, 152, 3, 107,
    12, 249, 242, 228, 106, 9, 139, 176,
    44, 54, 159, 188, 141, 254, 253, 35,
    26, 18, 141, 138, 75, 185, 173, 207,
    228, 94, 7, 24, 139, 117, 140, 58,
    211, 152, 215, 248, 78, 130, 239, 5
});

Signature signatureObject(g_signatureBytes);

std::string g_signatureHex = "0xaa42bbc2a9fc751beeee3b312b8452c445c7d4ab8698036b0cf9f2e46a098bb02c369fbc8dfefd231a128d8a4bb9adcfe45e07188b758c3ad398d7f84e82ef05";

CryptoPP::SecByteBlock g_signatureSerializedOutput = Utils::ByteVectorToSecBlock({
                                                                                  64, 170, 66, 187, 194, 169, 252, 117,
                                                                                  27, 238, 238, 59, 49, 43, 132, 82,
                                                                                  196, 69, 199, 212, 171, 134, 152, 3,
                                                                                  107, 12, 249, 242, 228, 106, 9, 139,
                                                                                  176, 44, 54, 159, 188, 141, 254, 253,
                                                                                  35, 26, 18, 141, 138, 75, 185, 173,
                                                                                  207, 228, 94, 7, 24, 139, 117, 140,
                                                                                  58, 211, 152, 215, 248, 78, 130, 239, 5 });

TEST(AccountTest, GeneratePrivateKeysWithBytesSuccess) {
    PrivateKey* privateKey = new PrivateKey(g_privateKeyBytes);
    ASSERT_TRUE(privateKey->KeyBytes().data() != nullptr);
    ASSERT_EQ(32, privateKey->KeyBytes().size());
    ASSERT_EQ(privateKey->KeyBytes(), g_privateKeyBytes);

    std::string privateKeyHex = privateKey->Key();
    ASSERT_EQ(privateKeyHex, g_privateKeyHex);
}

TEST(AccountTest, GenerateKeysWithBytesSuccess)
{
    PrivateKey* privateKey = new PrivateKey(g_privateKeyBytes);
    PublicKey* publicKey = new PublicKey(g_publicKeyBytes);

    ASSERT_TRUE(privateKey->KeyBytes().data() != nullptr);
    ASSERT_TRUE(publicKey->KeyBytes().data() != nullptr);

    std::string privateKeyHex = privateKey->ToString();
    std::string publicKeyHex = publicKey->ToString();

    ASSERT_EQ(privateKeyHex, g_privateKeyHex);
    ASSERT_EQ(publicKeyHex, g_publicKeyHex);
}

TEST(AccountTest, GenerateKeysWithStringSuccess)
{
    PrivateKey* privateKey = new PrivateKey(g_privateKeyHex);
    PublicKey* publicKey = new PublicKey(g_publicKeyHex);

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
    PrivateKey* privateKey = new PrivateKey(g_privateKeyHex);
    PublicKey publicKey = privateKey->GetPublicKey();

    ASSERT_EQ(g_publicKeyBytes, publicKey.KeyBytes());
}

TEST(AccountTest, PrivateKeyFromHexSignSuccess)
{
    PrivateKey * privateKey = new PrivateKey(g_privateKeyHex);
    ASSERT_EQ(privateKey->Key(), g_privateKeyHex);
    ASSERT_EQ(privateKey->KeyBytes(), g_privateKeyBytes);

    Signature signature = privateKey->Sign(g_messageUtf8Bytes);
    ASSERT_EQ(signature, signatureObject);
}

TEST(AccountTest, PrivateKeyFromBytesSignSuccess)
{
    PrivateKey privateKey(g_privateKeyBytes);
    ASSERT_EQ(privateKey.Key(), g_privateKeyHex);
    ASSERT_EQ(privateKey.KeyBytes(), g_privateKeyBytes);

    Signature signature = privateKey.Sign(g_messageUtf8Bytes);
    ASSERT_EQ(signature, signatureObject);
}

TEST(AccountTest, InvalidKeyGeneration)
{
    ASSERT_THROW( {
        PrivateKey* privateKey = new PrivateKey(g_privateKeyBytesInvalid);
        PublicKey* publicKey = new PublicKey(g_publicKeyBytesInvalid);
    },std::invalid_argument);
}

TEST(AccountTest, PublicKeySerialization)
{
    Serialization serializer;
    PublicKey* publicKey = new PublicKey(g_publicKeyBytes);
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
    Signature* sigOne = new Signature(g_signatureBytes);
    Signature* sigTwo = new Signature(g_signatureBytes);
    ASSERT_TRUE(sigOne->Equals(*sigTwo));
}

TEST(AccountTest, SignatureSerialization)
{
    Serialization serializer;
    Signature sig(g_signatureBytes);
    sig.Serialize(serializer);
    CryptoPP::SecByteBlock output = Utils::ByteVectorToSecBlock(serializer.GetBytes());
    ASSERT_EQ(output, g_signatureSerializedOutput);
}

TEST(AccountTest, SignatureDeserialization)
{
    Serialization serializer;
    Signature sig(g_signatureBytes);
    sig.Serialize(serializer);
    CryptoPP::SecByteBlock output = Utils::ByteVectorToSecBlock(serializer.GetBytes());

    Deserialization deser(Utils::SecBlockToByteVector(output));
    auto actualSig = std::dynamic_pointer_cast<Signature>(Signature::Deserialize(deser));
    ASSERT_EQ(sig, *actualSig);
}

TEST(AccountTests, GenerateAccountAddressFromPublicKey) {
    PublicKey publicKey(g_publicKeyBytes);
    AccountAddress accountAddress = AccountAddress::FromKey(publicKey.KeyBytes());
    ASSERT_EQ(accountAddress.ToString(), g_accountAddressHex);
}


TEST(AccountTests, CreateAccountFromKeys) {
    Account acc(g_privateKeyBytes, g_publicKeyBytes);
    auto privateKey = acc.getPrivateKey();
    auto publicKey = acc.getPublicKey();

    PublicKey validPublicKey(g_publicKeyBytes);
    PrivateKey validPrivateKey(g_privateKeyBytes);

    ASSERT_TRUE(privateKey->Key() == validPrivateKey.Key());
    ASSERT_TRUE(publicKey->Key() == validPublicKey.Key());
}

TEST(AccountTests, GenerateAccountFromPrivateKeyStringSuccess) {
    Account acc = Account::LoadKey(g_privateKeyHex);
    auto privateKey = acc.getPrivateKey();
    auto publicKey = acc.getPublicKey();

    PublicKey expectedPublicKey(g_publicKeyBytes);
    PrivateKey expectedPrivateKey(g_privateKeyBytes);

    ASSERT_TRUE(privateKey->Key() == expectedPrivateKey.Key());
    ASSERT_TRUE(publicKey->Key() == expectedPublicKey.Key());

    ASSERT_EQ(g_accountAddress, acc.getAccountAddress()->ToString());
}
TEST(AccountTests, CreateDefaultAccountSuccess) {
    Account acc = Account();
    ASSERT_TRUE(acc.getPrivateKey() != nullptr);
    ASSERT_TRUE(acc.getPublicKey() != nullptr);
    ASSERT_TRUE(acc.getAccountAddress() != nullptr);
}

TEST(AccountTests, InvalidAccountCreationWithShorterKeys) {
    ASSERT_THROW(Account acc(g_privateKeyBytesInvalid, g_publicKeyBytesInvalid), std::invalid_argument);
}

TEST(AccountTests, AuthKeyGeneration) {
    Account acc = Account(g_privateKeyBytes, g_publicKeyBytes);
    std::string authKey = acc.AuthKey();
    ASSERT_EQ(authKey, g_ccountAuthKeyHex);
}

TEST(AccountTests, AccountSignSuccess) {
    Account acc = Account(g_privateKeyBytes, g_publicKeyBytes);
    Signature signature = acc.Sign(g_messageUtf8Bytes);
    ASSERT_EQ(signature, signatureObject);
}

TEST(AccountTests, AccountSignVerify) {
    Account acc = Account(g_privateKeyBytes, g_publicKeyBytes);
    Signature signature = acc.Sign(g_messageUtf8Bytes);
    ASSERT_EQ(signature, signatureObject);
    bool verify = acc.Verify(g_messageUtf8Bytes, signature);
    ASSERT_TRUE(verify);
}

TEST(AccountTests, TestRotationProofChallenge) {
    ASSERT_EQ(true, false);

    //    Account OriginatingAccount = Account::LoadKey(
    //        "005120c5882b0d492b3d2dc60a8a4510ec2051825413878453137305ba2d644b"
    //        );

    //    Account TargetAccount = Account::LoadKey(
    //        "19d409c191b1787d5b832d780316b83f6ee219677fafbd4c0f69fee12fdcdcee"
    //        );

    //    RotationProofChallenge rotationProofChallenge = RotationProofChallenge(
    //        1234,
    //        OriginatingAccount.GetAccountAddress(),
    //        OriginatingAccount.GetAccountAddress(),
    //        TargetAccount.GetPublicKey().GetKeyBytes()
    //        );

    //    Serialization serializer;
    //    rotationProofChallenge.Serialize(serializer);
    //    std::string rotationProofChallengeBcs = Utils::HexStringFromByteArray(serializer.GetBytes());

    //    std::string expectedBytes =
    //        "0000000000000000000000000000000000000000000000000000000000000001" \
    //        "076163636f756e7416526f746174696f6e50726f6f664368616c6c656e6765d2" \
    //        "0400000000000015b67a673979c7c5dfc8d9c9f94d02da35062a19dd9d218087" \
    //        "bd9076589219c615b67a673979c7c5dfc8d9c9f94d02da35062a19dd9d218087" \
    //        "bd9076589219c620a1f942a3c46e2a4cd9552c0f95d529f8e3b60bcd44408637" \
    //        "9ace35e4458b9f22";

    //    ASSERT_EQ(rotationProofChallengeBcs, expectedBytes);
}


TEST(AccountTests, TestMultisig) {
    //Generate signatory private keys.
    PrivateKey privateKey1 = PrivateKey::FromHex(
        "4e5e3be60f4bbd5e98d086d932f3ce779ff4b58da99bf9e5241ae1212a29e5fe"
        );
    PrivateKey privateKey2 = PrivateKey::FromHex(
        "1e70e49b78f976644e2c51754a2f049d3ff041869c669523ba95b172c7329901"
        );

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
    std::string expectedPublicKeyBcs = "41754bb6a4720a658bdd5f532995955db0971ad3519acbde2f1149c3857348006c" \
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
    Signature signature = privateKey2.Sign(Utils::StringToSecByteBlock("multisig"));

    // Compose multisig signature.
    std::vector<std::tuple<PublicKey, Signature>> signMap = {
        std::make_tuple(privateKey2.GetPublicKey(), signature)
    };
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


TEST(AccountTests, TestMultiEd25519) {
    PrivateKey privateKey1 = PrivateKey::FromHex(
        "4e5e3be60f4bbd5e98d086d932f3ce779ff4b58da99bf9e5241ae1212a29e5fe"
        );

    PrivateKey privateKey2 = PrivateKey::FromHex(
        "1e70e49b78f976644e2c51754a2f049d3ff041869c669523ba95b172c7329901"
        );

    MultiPublicKey multiSigPublicKey = MultiPublicKey(std::vector<PublicKey> {privateKey1.GetPublicKey(), privateKey2.GetPublicKey()}, 1);

    AccountAddress expected = AccountAddress::FromHex("835bb8c5ee481062946b18bbb3b42a40b998d6bf5316ca63834c959dc739acf0");

    AccountAddress actual = AccountAddress::FromMultiEd25519(multiSigPublicKey);

    ASSERT_EQ(expected, actual);
}


TEST(AccountTests, TestResourceAccount) {
    AccountAddress baseAddress = AccountAddress::FromHex("b0b");
    AccountAddress expected = AccountAddress::FromHex("ee89f8c763c27f9d942d496c1a0dcf32d5eacfe78416f9486b8db66155b163b0");

    std::vector<uint8_t> seed = {0x0b, 0x00, 0x0b};
    AccountAddress actual = AccountAddress::ForResourceAccount(baseAddress, Utils::ByteVectorToSecBlock(seed));

    ASSERT_EQ(actual, expected);
}


TEST(AccountTests, TestNamedObject) {
    AccountAddress baseAddress = AccountAddress::FromHex("b0b");
    AccountAddress expected = AccountAddress::FromHex("f417184602a828a3819edf5e36285ebef5e4db1ba36270be580d6fd2d7bcc321");

    CryptoPP::SecByteBlock seed = Utils::ByteVectorToSecBlock(
        {'b', 'o', 'b', '\'', 's', ' ', 'c', 'o', 'l', 'l', 'e', 'c', 't', 'i', 'o', 'n'});
    AccountAddress actual = AccountAddress::ForNamedObject(baseAddress, seed);

    ASSERT_EQ(actual, expected);
}


TEST(AccountTests, TestCollection) {
    AccountAddress baseAddress = AccountAddress::FromHex("b0b");
    AccountAddress expected = AccountAddress::FromHex("f417184602a828a3819edf5e36285ebef5e4db1ba36270be580d6fd2d7bcc321");

    AccountAddress actual = AccountAddress::ForNamedCollection(baseAddress, "bob's collection");

    ASSERT_EQ(actual, expected);
}


TEST(AccountTests, TestToken) {
    AccountAddress baseAddress = AccountAddress::FromHex("b0b");
    AccountAddress expected = AccountAddress::FromHex("e20d1f22a5400ba7be0f515b7cbd00edc42dbcc31acc01e31128b2b5ddb3c56e");

    AccountAddress actual = AccountAddress::ForNamedToken(baseAddress, "bob's collection", "bob's token");

    ASSERT_EQ(actual, expected);
}



TEST(AccountTests, TestToStandardString) {
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
