#include "gtest/gtest.h"
#include "../Accounts/AccountAddress.h"
#include <string>

#include <cryptopp/cryptlib.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/secblock.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include "../BCS/Modules/ModuleId.h"
#include "../BCS/EntryFunctions/EntryFunction.h"
#include "../BCS/StructTag.h"
#include "../BCS/BCS.h"
#include "../Transactions/TransactionPayload.h"
#include "../Transactions/Rawtransaction.h"
#include "../Transactions/Authenticator.h"
#include "../Transactions/SignedTransaction.h"
#include "../Transactions/MultiAgentRawTransaction.h"
#include "../HDWallet/Utils/Utils.h"
#include <fstream>

using CryptoPP::byte;
using namespace Aptos;
using namespace Aptos::BCS;
using namespace Aptos::Accounts;
using namespace Aptos::Accounts::Ed25519;

AccountAddress testAddress()
{
    return AccountAddress::FromHex("0x01");
}

ModuleId testModuleId()
{
    return ModuleId(testAddress(), "my_module");
}

EntryFunction testEntryFunction(const std::vector<std::shared_ptr<ISerializableTag>> &typeTags,
                                std::vector<std::shared_ptr<ISerializable>> &args)
{
    return EntryFunction::Natural(testModuleId(), "some_function", TagSequence(typeTags), Sequence(args));
}

TEST(AddressSerialize, serializationTest)
{
    Serialization s;
    s.Serialize(testAddress());
    std::vector<byte> res = s.GetBytes();
    std::vector<byte> expected = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    ASSERT_EQ(expected, res);
}

TEST(MyModuleIdTests, ModuleIdSerializeTest)
{
    Serialization s;
    testModuleId().Serialize(s);

    const byte expectedBytes[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9, 109,
        121, 95, 109, 111, 100, 117, 108, 101};

    auto res = s.GetBytes();

    for (int i = 0; i < sizeof(expectedBytes); ++i)
    {
        EXPECT_EQ(expectedBytes[i], res[i]);
    }
}

TEST(Transaction_Simple_Serialize, TestName)
{
    Serialization s;
    std::vector<std::shared_ptr<ISerializableTag>> tags;
    std::vector<std::shared_ptr<ISerializable>> args;

    testEntryFunction(tags, args).Serialize(s);
    s.GetBytes();
    std::vector<uint8_t> res = s.GetBytes();

    std::vector<uint8_t> expected = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9, 109,
        121, 95, 109, 111, 100, 117, 108, 101, 13, 115, 111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0, 0};

    ASSERT_EQ(expected, res);
}

TEST(Transaction_EmptyArgSequence_Serialize, TestName)
{
    Serialization s;
    std::vector<std::shared_ptr<ISerializableTag>> tags;
    std::vector<std::shared_ptr<ISerializable>> args;

    testEntryFunction(tags, args).Serialize(s);

    s.GetBytes();
    std::vector<uint8_t> res = s.GetBytes();

    std::vector<uint8_t> expected = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9, 109,
        121, 95, 109, 111, 100, 117, 108, 101, 13, 115, 111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0, 0};

    ASSERT_EQ(expected, res);
}

TEST(SequenceBStringSerializeTest, SequenceBStringSerialize)
{
    Serialization s;
    Sequence sequenceEmptyBString{{std::make_shared<BString>("")}};
    sequenceEmptyBString.Serialize(s);
    std::vector<uint8_t> res = s.GetBytes();
    std::vector<uint8_t> expectedBytes1 = {1, 1, 0};
    EXPECT_EQ(res, expectedBytes1);

    // Serialize a BString with one character 'A'
    s = Serialization();
    Sequence sequenceBStringA{{std::make_shared<BString>("A")}};
    sequenceBStringA.Serialize(s);
    res = s.GetBytes();
    std::vector<uint8_t> expectedBytes2 = {1, 2, 1, 65};
    EXPECT_EQ(res, expectedBytes2);

    // Serialize a BString with two characters 'AA'
    s = Serialization();
    Sequence sequenceBStringAA{{std::make_shared<BString>("AA")}};
    sequenceBStringAA.Serialize(s);
    res = s.GetBytes();
    std::vector<uint8_t> expectedBytes3 = {1, 3, 2, 65, 65};
    EXPECT_EQ(res, expectedBytes3);
}

TEST(SequenceOfSequenceSerializeTest, SequenceOfSequenceSerialize)
{
    Serialization s;
    Sequence innerSequence{{}};
    Sequence outerSequence{{std::make_shared<Sequence>(innerSequence)}};
    outerSequence.Serialize(s);

    std::vector<uint8_t> actual = s.GetBytes();
    std::vector<uint8_t> expectedBytes = {1, 1, 0};
    EXPECT_EQ(actual, expectedBytes);
}

TEST(TransactionSingleStringArgSerializeTest, TransactionSingleStringArgSerialize)
{
    Serialization s;
    std::vector<std::shared_ptr<ISerializable>> args;
    args.push_back(std::make_shared<BString>("wow"));
    std::vector<std::shared_ptr<ISerializableTag>> empt;

    testEntryFunction({}, args).Serialize(s);
    std::vector<uint8_t> actual = s.GetBytes();
    std::vector<uint8_t> expectedBytes = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9, 109, 121, 95, 109, 111, 100, 117, 108, 101, 13, 115, 111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0, 1, 4, 3, 119, 111, 119};

    EXPECT_EQ(actual, expectedBytes);
}

TEST(TransactionSingleU64ArgSerializeTest, TransactionSingleU64ArgSerialize)
{
    Serialization s;
    std::vector<std::shared_ptr<ISerializable>> args;
    args.push_back(std::make_shared<U64>(555555));
    std::vector<std::shared_ptr<ISerializableTag>> empt;

    testEntryFunction(empt, args).Serialize(s);

    std::vector<uint8_t> actual = s.GetBytes();
    std::vector<uint8_t> expectedBytes = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9, 109, 121, 95, 109, 111, 100, 117, 108, 101, 13, 115, 111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0, 1, 8, 35, 122, 8, 0, 0, 0, 0, 0};

    EXPECT_EQ(actual, expectedBytes);
}

TEST(TransactionWithEmptyBoolArgSequenceSerializeTest, TransactionWithEmptyBoolArgSequenceSerialize)
{
    std::vector<std::shared_ptr<ISerializable>> boolSequence;
    Serialization s;
    std::vector<std::shared_ptr<ISerializable>> args = {std::make_shared<Sequence>(boolSequence)};

    testEntryFunction({}, args).Serialize(s);

    const std::vector<uint8_t> actualVec = s.GetBytes();
    const std::vector<uint8_t> expected = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        9, 109, 121, 95, 109, 111, 100, 117, 108, 101, 13, 115,
        111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0,
        1, 1, 0};

    ASSERT_EQ(expected, actualVec);
}

TEST(TransactionOneBoolArgSequenceSerializeTest, TransactionOneBoolArgSequenceSerialize)
{
    std::vector<std::shared_ptr<ISerializable>> boolSequence = {std::make_shared<Bool>(false)};
    Serialization s;
    std::vector<std::shared_ptr<ISerializable>> args = {std::make_shared<Sequence>(boolSequence)};

    testEntryFunction({}, args).Serialize(s);

    const std::vector<uint8_t> actualVec = s.GetBytes();
    const std::vector<uint8_t> expected = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        9, 109, 121, 95, 109, 111, 100, 117, 108, 101, 13, 115,
        111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0,
        1, 2, 1, 0};

    ASSERT_EQ(expected, actualVec);
}

TEST(TransactionTwoBoolSequenceSerializeTest, TransactionTwoBoolSequenceSerialize)
{
    std::vector<std::shared_ptr<ISerializable>> boolSequence = {std::make_shared<Bool>(false), std::make_shared<Bool>(true)};
    Serialization s;
    std::vector<std::shared_ptr<ISerializable>> args = {std::make_shared<Sequence>(boolSequence)};

    testEntryFunction({}, args).Serialize(s);

    const std::vector<uint8_t> actualVec = s.GetBytes();
    const std::vector<uint8_t> expected = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        9, 109, 121, 95, 109, 111, 100, 117, 108, 101, 13, 115,
        111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0,
        1, 3, 2, 0, 1};

    ASSERT_EQ(expected, actualVec);
}

TEST(TransactionThreeBoolArgsSequenceSerializeTest, TransactionThreeBoolArgsSequenceSerialize)
{
    std::vector<std::shared_ptr<ISerializable>> boolSequence = {std::make_shared<Bool>(false),
                                                                std::make_shared<Bool>(true),
                                                                std::make_shared<Bool>(false)};
    Serialization s;
    std::vector<std::shared_ptr<ISerializable>> args = {std::make_shared<Sequence>(boolSequence)};

    testEntryFunction({}, args).Serialize(s);

    const std::vector<uint8_t> actualVec = s.GetBytes();
    const std::vector<uint8_t> expected = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        9, 109, 121, 95, 109, 111, 100, 117, 108, 101, 13, 115,
        111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0,
        1, 4, 3, 0, 1, 0};

    ASSERT_EQ(expected, actualVec);
}

TEST(TransactionWithOneStringArgSequenceSerializeTest, TransactionWithOneStringArgSequenceSerialize)
{
    Serialization s;
    std::vector<std::shared_ptr<ISerializable>> args = {std::make_shared<Sequence>(
        std::vector<std::shared_ptr<ISerializable>>{std::make_shared<BString>("A")})};

    testEntryFunction({}, args).Serialize(s);

    const std::vector<uint8_t> actualVec = s.GetBytes();
    const std::vector<uint8_t> expected = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9, 109, 121, 95, 109, 111, 100, 117, 108, 101, 13, 115, 111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0, 1, 3, 1, 1, 65};

    ASSERT_EQ(expected, actualVec);
}

TEST(TransactionWithTwoStringArgSequenceSerializeTest, TransactionWithTwoStringArgSequenceSerialize)
{
    Serialization s;
    std::vector<std::shared_ptr<ISerializable>> args = {std::make_shared<Sequence>(
        std::vector<std::shared_ptr<ISerializable>>{
            std::make_shared<BString>("A"),
            std::make_shared<BString>("B")})};

    testEntryFunction({}, args).Serialize(s);

    const std::vector<uint8_t> actualVec = s.GetBytes();
    const std::vector<uint8_t> expected = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9, 109, 121, 95, 109, 111, 100, 117, 108, 101, 13, 115, 111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0, 1, 5, 2, 1, 65, 1, 66};

    ASSERT_EQ(expected, actualVec);
}

TEST(TransactionThreeStringArgSequenceSerializeTest, TransactionThreeStringArgSequenceSerialize)
{
    Serialization s;
    std::vector<std::shared_ptr<ISerializable>> args = {std::make_shared<Sequence>(
        std::vector<std::shared_ptr<ISerializable>>{
            std::make_shared<BString>("A"),
            std::make_shared<BString>("B"),
            std::make_shared<BString>("C")})};
    testEntryFunction({}, args).Serialize(s);

    const std::vector<uint8_t> actualVec = s.GetBytes();
    const std::vector<uint8_t> expected = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9, 109, 121, 95, 109, 111, 100, 117, 108, 101, 13, 115, 111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0, 1, 7, 3, 1, 65, 1, 66, 1, 67};

    ASSERT_EQ(expected, actualVec);
}

TEST(TransactioOneStringOneBoolArgSequenceSerializeTest, TransactioOneStringOneBoolArgSequenceSerialize)
{
    std::vector<std::shared_ptr<ISerializable>> boolSequence = {std::make_shared<Bool>(false)};
    Serialization s;
    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<BString>("A"),
        std::make_shared<Sequence>(boolSequence)};
    testEntryFunction({}, args).Serialize(s);

    const std::vector<uint8_t> actualVec = s.GetBytes();
    const std::vector<uint8_t> expected = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        9, 109, 121, 95, 109, 111, 100, 117, 108, 101, 13, 115,
        111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0,
        2, 2, 1, 65, 2, 1, 0};
    ASSERT_EQ(expected, actualVec);
}

TEST(TransactionOneStringOneIntOneBoolArgSequenceSerializeTest, TransactionOneStringOneIntOneBoolArgSequenceSerialize)
{
    std::vector<std::shared_ptr<ISerializable>> boolSequence = {std::make_shared<Bool>(false)};
    Serialization s;
    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<BString>("A"),
        std::make_shared<U64>(1),
        std::make_shared<Sequence>(boolSequence)};
    testEntryFunction({}, args).Serialize(s);

    const std::vector<uint8_t> actualVec = s.GetBytes();
    const std::vector<uint8_t> expected = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        9, 109, 121, 95, 109, 111, 100, 117, 108, 101, 13, 115,
        111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0,
        3, 2, 1, 65, 8, 1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0};

    ASSERT_EQ(expected, actualVec);
}

TEST(TransactionOneStringOneIntOneAddressOneBoolArgSequenceSerializeTest, TransactionOneStringOneIntOneAddressOneBoolArgSequenceSerialize)
{
    std::vector<std::shared_ptr<ISerializable>> boolSequence = {std::make_shared<Bool>(false)};
    Serialization s;
    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<BString>("A"),
        std::make_shared<U64>(1),
        std::make_shared<AccountAddress>(testAddress()),
        std::make_shared<Sequence>(boolSequence)};
    testEntryFunction({}, args).Serialize(s);

    const std::vector<uint8_t> actualVec = s.GetBytes();
    const std::vector<uint8_t> expected = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9, 109, 121, 95, 109, 111, 100, 117, 108, 101, 13, 115, 111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0, 4, 2, 1, 65, 8, 1, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0};

    ASSERT_EQ(expected, actualVec);
}

TEST(TransactionOneStringOneIntOneAddressMultipleArgSequencesSerializeTest, TransactionOneStringOneIntOneAddressMultipleArgSequencesSerialize)
{
    std::vector<std::shared_ptr<ISerializable>> boolSequence1 = {std::make_shared<Bool>(false)};
    std::vector<std::shared_ptr<ISerializable>> boolSequence2 = {
        std::make_shared<Bool>(false),
        std::make_shared<Bool>(true),
    };
    std::vector<std::shared_ptr<ISerializable>> strSequence = {std::make_shared<BString>("A"),
                                                               std::make_shared<BString>("B"),
                                                               std::make_shared<BString>("C")};

    Serialization s;
    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<BString>("A"),
        std::make_shared<U64>(1),
        std::make_shared<AccountAddress>(testAddress()),
        std::make_shared<Sequence>(boolSequence1),
        std::make_shared<Sequence>(boolSequence2),
        std::make_shared<Sequence>(strSequence)};
    testEntryFunction({}, args).Serialize(s);

    const std::vector<uint8_t> actualVec = s.GetBytes();
    const std::vector<uint8_t> expected = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9, 109, 121, 95, 109, 111, 100, 117, 108, 101, 13, 115, 111, 109, 101, 95, 102, 117, 110, 99, 116, 105, 111, 110, 0, 6, 2, 1, 65, 8, 1, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 3, 2, 0, 1, 7, 3, 1, 65, 1, 66, 1, 67};

    ASSERT_EQ(expected, actualVec);
}

TEST(ModuleFromStringTest, ModuleFromString)
{
    AccountAddress accountAddress = AccountAddress::FromHex("0x1");
    std::string name = "coin";
    ModuleId expectedModuleId(accountAddress, name);
    ModuleId actualModuleId = ModuleId::FromStr("0x1::coin");
    EXPECT_EQ(expectedModuleId, actualModuleId);
}

TEST(ModuleIdTest, ToString)
{
    // Create a ModuleId object
    AccountAddress accountAddress = AccountAddress::FromHex("0x1");
    std::string name = "coin";
    ModuleId moduleId(accountAddress, name);

    // Call ToString() method
    std::string actualString = moduleId.ToString();

    // Check if the resulting string matches the expected format
    std::string expectedString = "0x1::coin";
    EXPECT_EQ(expectedString, actualString);
}

TEST(ModuleIdTest, GetHashCode)
{
    // Create a ModuleId object
    AccountAddress accountAddress = AccountAddress::FromHex("0x1");
    std::string name = "coin";
    ModuleId moduleId(accountAddress, name);

    // Call GetHashCode() method
    size_t actualHash = moduleId.GetHashCode();

    // Check if the resulting hash is 0
    EXPECT_EQ(0, actualHash);
}

TEST(ModuleIdTest, EqualityOperator)
{
    // Create two identical ModuleId objects
    AccountAddress accountAddress1 = AccountAddress::FromHex("0x1");
    std::string name1 = "coin";
    ModuleId moduleId1(accountAddress1, name1);
    ModuleId moduleId2(accountAddress1, name1);

    // Check if the two identical ModuleId objects are equal
    EXPECT_TRUE(moduleId1 == moduleId2);

    // Create two different ModuleId objects
    AccountAddress accountAddress3 = AccountAddress::FromHex("0x2");
    std::string name3 = "token";
    ModuleId moduleId3(accountAddress3, name3);

    // Check if the two different ModuleId objects are not equal
    EXPECT_FALSE(moduleId1 == moduleId3);
}

TEST(ModuleIdTest, Constructor)
{
    // Create an AccountAddress object and a name string
    AccountAddress accountAddress = AccountAddress::FromHex("0x1");
    std::string name = "coin";

    // Create a ModuleId object using the AccountAddress object and the name string
    ModuleId moduleId(accountAddress, name);

    // Check if the ToString method of the ModuleId object returns the correct string
    std::string expectedString = "0x1::coin";
    EXPECT_EQ(expectedString, moduleId.ToString());
}

TEST(ModuleIdTest, Equals)
{
    // Create two identical ModuleId objects
    AccountAddress accountAddress1 = AccountAddress::FromHex("0x1");
    std::string name1 = "coin";
    ModuleId moduleId1(accountAddress1, name1);

    AccountAddress accountAddress2 = AccountAddress::FromHex("0x1");
    std::string name2 = "coin";
    ModuleId moduleId2(accountAddress2, name2);

    // Check if Equals returns true for identical objects
    EXPECT_TRUE(moduleId1.Equals(moduleId2));

    // Modify one of the ModuleId objects
    AccountAddress accountAddress3 = AccountAddress::FromHex("0x2");
    ModuleId moduleId3(accountAddress3, name1);

    // Check if Equals returns false for different objects
    EXPECT_FALSE(moduleId1.Equals(moduleId3));

    std::string name3 = "coin2";
    ModuleId moduleId4(accountAddress1, name3);

    // Check if Equals returns false for different objects
    EXPECT_FALSE(moduleId1.Equals(moduleId4));
}

TEST(ModuleIdTest, FromStr)
{
    // Test with a valid ModuleId string
    std::string validModuleIdStr = "0x1::coin";
    ModuleId validModuleId = ModuleId::FromStr(validModuleIdStr);
    // Test with an empty string
    std::string emptyStr = "";
    EXPECT_THROW(ModuleId::FromStr(emptyStr), std::invalid_argument);

    // Test with a string with no "::"
    std::string noSeparatorStr = "0x1coin";
    EXPECT_THROW(ModuleId::FromStr(noSeparatorStr), std::invalid_argument);

    // Test with a string with "::" at the beginning
    std::string startSeparatorStr = "::coin";
    EXPECT_THROW(ModuleId::FromStr(startSeparatorStr), std::invalid_argument);

    // Test with a string with "::" at the end
    std::string endSeparatorStr = "0x1::";
    EXPECT_THROW(ModuleId::FromStr(endSeparatorStr), std::invalid_argument);
}

TEST(StructTagFromStringTest, StructTagFromString)
{
    AccountAddress accountAddress = AccountAddress::FromHex("0x1");
    std::string module = "aptos_coin";
    std::string name = "AptosCoin";
    std::vector<std::shared_ptr<ISerializableTag>> tags;

    StructTag expectedStructTag(accountAddress, module, name, tags);
    StructTag actualStructTag = StructTag::FromStr("0x1::aptos_coin::AptosCoin");

    EXPECT_EQ(expectedStructTag, actualStructTag);
}

TEST(TagSequenceSerializeTest, TagSequenceSerialize)
{
    AccountAddress accountAddress = AccountAddress::FromHex("0x1");
    std::string module = "aptos_coin";
    std::string name = "AptosCoin";
    std::vector<std::shared_ptr<ISerializableTag>> tags;

    StructTag structTag(accountAddress, module, name, tags);
    std::vector<std::shared_ptr<ISerializableTag>> typeTags = {std::make_shared<StructTag>(structTag)};

    TagSequence typeTagsSequence(typeTags);

    // Serialize the TagSequence
    Serialization ser;
    typeTagsSequence.Serialize(ser);
    std::vector<uint8_t> typeTagsBytes = ser.GetBytes();

    // Deserialize the serialized data
    Deserialization deser(typeTagsBytes);
    auto actualTagSeq = std::dynamic_pointer_cast<TagSequence>(TagSequence::Deserialize(deser));
    EXPECT_EQ(typeTagsSequence, *actualTagSeq);
}

TEST(EntryFunctionSerializeTest, EntryFunction_PayloadForTransferCoin_Serialize)
{
    TagSequence typeTags{std::vector<std::shared_ptr<ISerializableTag>>{std::make_shared<StructTag>(
        AccountAddress::FromHex("0x1"),
        "aptos_coin",
        "AptosCoin",
        std::vector<std::shared_ptr<ISerializableTag>>{})}};

    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<AccountAddress>(AccountAddress::FromHex("0x1")),
        std::make_shared<U64>(1000)};

    Sequence txnArgs(args);

    EntryFunction payload = EntryFunction::Natural(
        ModuleId(AccountAddress::FromHex("0x1"), "coin"),
        "transfer",
        typeTags,
        txnArgs);

    Serialization s;
    payload.Serialize(s);

    std::vector<uint8_t> actual = s.GetBytes();
    std::vector<uint8_t> expected = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 99, 111, 105, 110, 8, 116, 114, 97, 110, 115, 102, 101, 114, 1, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 10, 97, 112, 116, 111, 115, 95, 99, 111, 105, 110, 9, 65, 112, 116, 111, 115, 67, 111, 105, 110, 0, 2, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8, 232, 3, 0, 0, 0, 0, 0, 0};

    ASSERT_EQ(expected, actual);
    ASSERT_NE(payload.GetHashCode(), 0);
}

TEST(ScriptSerializeTest, ScriptSerialize)
{
    std::string path = "two_by_two_transfer.mv"; // Update with your actual script path
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file: " << path << std::endl;
        ASSERT_EQ(true, false);
        return;
    }
    std::vector<uint8_t> data;
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    data.resize(file_size);

    if (file.read(reinterpret_cast<char *>(data.data()), file_size))
    {
        std::cout << "File read successfully. Size: " << data.size() << " bytes." << std::endl;
    }
    else
    {
        std::cerr << "Failed to read the file." << std::endl;
        ASSERT_EQ(true, false);
        return;
    }

    AccountAddress caroldAddress = AccountAddress::FromHex("0x1");
    AccountAddress davidAddress = AccountAddress::FromHex("0x1");
    TagSequence typeArgs{std::vector<std::shared_ptr<ISerializableTag>>{}};

    auto scriptArgU64_1 = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::U64, std::make_shared<U64>(100));
    auto scriptArgU64_2 = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::U64, std::make_shared<U64>(200));
    auto scriptArgAddress_1 = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::ACCOUNT_ADDRESS, std::make_shared<AccountAddress>(caroldAddress));
    auto scriptArgAddress_2 = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::ACCOUNT_ADDRESS, std::make_shared<AccountAddress>(davidAddress));
    auto scriptArgU64_3 = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::U64, std::make_shared<U64>(50));

    std::vector<std::shared_ptr<ISerializable>> args = {
        scriptArgU64_1, scriptArgU64_2, scriptArgAddress_1, scriptArgAddress_2, scriptArgU64_3};
    Sequence scriptArgs(args);

    Script script(data, typeArgs, scriptArgs);

    Serialization ser;
    script.Serialize(ser);
    std::vector<uint8_t> actual = ser.GetBytes();

    std::vector<uint8_t> expected = {187, 2, 161, 28, 235, 11, 5, 0, 0, 0, 8, 1, 0, 4, 2, 4, 10, 3, 14, 24, 4, 38, 8, 5, 46, 67, 7, 113, 62, 8, 175, 1, 32, 6, 207, 1, 20, 0, 0, 0, 1, 1, 2, 4, 1, 0, 1, 0, 3, 8, 0, 1, 4, 3, 4, 1, 0, 1, 5, 5, 6, 1, 0, 1, 6, 7, 4, 1, 0, 1, 7, 8, 6, 1, 0, 0, 2, 1, 2, 2, 2, 3, 2, 7, 6, 12, 6, 12, 3, 3, 5, 5, 3, 3, 11, 0, 1, 8, 1, 11, 0, 1, 8, 1, 11, 0, 1, 8, 1, 1, 8, 1, 2, 6, 12, 3, 1, 11, 0, 1, 9, 0, 2, 7, 11, 0, 1, 9, 0, 11, 0, 1, 9, 0, 0, 2, 7, 11, 0, 1, 9, 0, 3, 2, 5, 11, 0, 1, 9, 0, 10, 97, 112, 116, 111, 115, 95, 99, 111, 105, 110, 4, 99, 111, 105, 110, 4, 67, 111, 105, 110, 9, 65, 112, 116, 111, 115, 67, 111, 105, 110, 8, 119, 105, 116, 104, 100, 114, 97, 119, 5, 109, 101, 114, 103, 101, 7, 101, 120, 116, 114, 97, 99, 116, 7, 100, 101, 112, 111, 115, 105, 116, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 8, 160, 134, 1, 0, 0, 0, 0, 0, 3, 8, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 1, 26, 11, 0, 10, 2, 56, 0, 12, 7, 11, 1, 10, 3, 56, 0, 12, 8, 13, 7, 11, 8, 56, 1, 13, 7, 11, 2, 11, 3, 22, 11, 6, 23, 56, 2, 12, 9, 11, 4, 11, 7, 56, 3, 11, 5, 11, 9, 56, 3, 2, 0, 5, 1, 100, 0, 0, 0, 0, 0, 0, 0, 1, 200, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 50, 0, 0, 0, 0, 0, 0, 0};

    ASSERT_EQ(expected, actual);

    Deserialization deser(actual);
    auto actualScript = std::dynamic_pointer_cast<Script>(Script::Deserialize(deser));
    ASSERT_EQ(script, *actualScript);
}

TEST(ScriptTest, ToString)
{
    // Create a ISerializable object with a known ToString value
    TagSequence typeTags{std::vector<std::shared_ptr<ISerializableTag>>{std::make_shared<StructTag>(AccountAddress::FromHex("0x1"), "aptos_coin", "AptosCoin", std::vector<std::shared_ptr<ISerializableTag>>{})}};

    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<AccountAddress>(AccountAddress::FromHex("0x1")),
        std::make_shared<U64>(1000),
    };
    Sequence scriptArgs(args);

    std::vector<uint8_t> code; // Filled with some arbitrary bytecode

    // Create a Script object with the known ToString value
    Script script(code, typeTags, scriptArgs);

    // Call the ToString method
    std::string returned_str = script.ToString();

    // Known value is updated to the expected string
    std::string known_value = "<0x1::aptos_coin::AptosCoin>(0x11000)";

    // Assert that the returned string is the same as the string representation of the known ToString value
    EXPECT_EQ(returned_str, known_value);
}

TEST(ScriptTest, EqualsMethod)
{
    // Create two identical Script objects
    TagSequence typeTags{std::vector<std::shared_ptr<ISerializableTag>>{std::make_shared<StructTag>(AccountAddress::FromHex("0x1"), "aptos_coin", "AptosCoin", std::vector<std::shared_ptr<ISerializableTag>>{})}};

    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<AccountAddress>(AccountAddress::FromHex("0x1")),
        std::make_shared<U64>(1000),
    };
    Sequence scriptArgs(args);

    std::vector<uint8_t> code = {1, 2, 3, 4, 5}; // Some arbitrary bytecode

    // Create two identical Script objects
    Script script1(code, typeTags, scriptArgs);
    Script script2(code, typeTags, scriptArgs);

    // Assert that the two scripts are equal
    EXPECT_TRUE(script1.Equals(script2));

    // Now alter one of the scripts
    std::vector<uint8_t> codeDifferent = {5, 4, 3, 2, 1}; // Different arbitrary bytecode
    Script script3(codeDifferent, typeTags, scriptArgs);

    // Assert that the scripts are no longer equal
    EXPECT_FALSE(script1.Equals(script3));
}

TEST(ScriptTest, GetHashCodeMethod)
{
    // Create a Script object
    TagSequence typeTags{std::vector<std::shared_ptr<ISerializableTag>>{std::make_shared<StructTag>(AccountAddress::FromHex("0x1"), "aptos_coin", "AptosCoin", std::vector<std::shared_ptr<ISerializableTag>>{})}};

    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<AccountAddress>(AccountAddress::FromHex("0x1")),
        std::make_shared<U64>(1000),
    };
    Sequence scriptArgs(args);

    std::vector<uint8_t> code = {1, 2, 3, 4, 5}; // Some arbitrary bytecode

    // Create a Script object
    Script script(code, typeTags, scriptArgs);

    // Call GetHashCode() method
    size_t actualHash = script.GetHashCode();

    // Calculate expected hash
    size_t expectedHash = 17;
    for (uint8_t byte : code)
    {
        expectedHash = expectedHash * 23 + static_cast<size_t>(byte);
    }
    expectedHash = expectedHash * 23 + typeTags.GetHashCode();
    expectedHash = expectedHash * 23 + scriptArgs.GetHashCode();

    // Check if the resulting hash is as expected
    EXPECT_EQ(actualHash, expectedHash);
}

TEST(TransactionPayloadSerializeTest, Transaction_PayloadForTransferCoin_Serialize)
{
    TagSequence typeTags{std::vector<std::shared_ptr<ISerializableTag>>{std::make_shared<StructTag>(AccountAddress::FromHex("0x1"), "aptos_coin", "AptosCoin", std::vector<std::shared_ptr<ISerializableTag>>{})}};

    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<AccountAddress>(AccountAddress::FromHex("0x1")),
        std::make_shared<U64>(1000),
    };
    Sequence txnArgs(args);

    ModuleId moduleId(AccountAddress::FromHex("0x1"), "coin");
    EntryFunction payload = EntryFunction::Natural(moduleId, "transfer", typeTags, txnArgs);

    TransactionPayload txnPayload(std::make_shared<EntryFunction>(payload));

    Serialization ser;
    txnPayload.Serialize(ser);
    std::vector<uint8_t> actual = ser.GetBytes();
    std::vector<uint8_t> expected = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 99, 111, 105, 110, 8, 116, 114, 97, 110, 115, 102, 101, 114, 1, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 10, 97, 112, 116, 111, 115, 95, 99, 111, 105, 110, 9, 65, 112, 116, 111, 115, 67, 111, 105, 110, 0, 2, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8, 232, 3, 0, 0, 0, 0, 0, 0};

    ASSERT_EQ(expected, actual);
}

TEST(TransactionPayloadTest, ToString)
{
    // Create a ISerializable object with a known ToString value
    TagSequence typeTags{std::vector<std::shared_ptr<ISerializableTag>>{std::make_shared<StructTag>(AccountAddress::FromHex("0x1"), "aptos_coin", "AptosCoin", std::vector<std::shared_ptr<ISerializableTag>>{})}};

    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<AccountAddress>(AccountAddress::FromHex("0x1")),
        std::make_shared<U64>(1000),
    };
    Sequence txnArgs(args);

    ModuleId moduleId(AccountAddress::FromHex("0x1"), "coin");
    EntryFunction payload = EntryFunction::Natural(moduleId, "transfer", typeTags, txnArgs);

    // Create a TransactionPayload object with the known ToString value
    TransactionPayload transactionPayload(std::make_shared<EntryFunction>(payload));

    // Call the ToString method
    std::string returned_str = transactionPayload.ToString();

    // Known value is updated to the expected string
    std::string known_value = "0x1::coin::transfer<0x1::aptos_coin::AptosCoin>(000000000000000000000000000000012323000000)";

    // Assert that the returned string is the same as the string representation of the known ToString value
    EXPECT_EQ(returned_str, known_value);
}

TEST(TransactionPayloadTest, EqualsMethod)
{
    // Create two identical TransactionPayload objects
    TagSequence typeTags{std::vector<std::shared_ptr<ISerializableTag>>{std::make_shared<StructTag>(AccountAddress::FromHex("0x1"), "aptos_coin", "AptosCoin", std::vector<std::shared_ptr<ISerializableTag>>{})}};

    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<AccountAddress>(AccountAddress::FromHex("0x1")),
        std::make_shared<U64>(1000),
    };
    Sequence txnArgs(args);

    ModuleId moduleId(AccountAddress::FromHex("0x1"), "coin");
    EntryFunction payload = EntryFunction::Natural(moduleId, "transfer", typeTags, txnArgs);

    TransactionPayload payload1(std::make_shared<EntryFunction>(payload));
    TransactionPayload payload2(std::make_shared<EntryFunction>(payload));

    // Assert that the two payloads are equal
    EXPECT_TRUE(payload1.Equals(payload2));

    // Now alter one of the payloads
    ModuleId moduleIdDifferent(AccountAddress::FromHex("0x2"), "coin");
    EntryFunction payloadDifferent = EntryFunction::Natural(moduleIdDifferent, "transfer", typeTags, txnArgs);
    TransactionPayload payload3(std::make_shared<EntryFunction>(payloadDifferent));

    // Assert that the payloads are no longer equal
    EXPECT_FALSE(payload1.Equals(payload3));
}

TEST(TransactionPayloadTest, GetHashCodeMethod)
{
    // Create a TransactionPayload object
    TagSequence typeTags{std::vector<std::shared_ptr<ISerializableTag>>{std::make_shared<StructTag>(AccountAddress::FromHex("0x1"), "aptos_coin", "AptosCoin", std::vector<std::shared_ptr<ISerializableTag>>{})}};

    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<AccountAddress>(AccountAddress::FromHex("0x1")),
        std::make_shared<U64>(1000),
    };
    Sequence txnArgs(args);

    ModuleId moduleId(AccountAddress::FromHex("0x1"), "coin");
    EntryFunction entryFunction1 = EntryFunction::Natural(moduleId, "transfer", typeTags, txnArgs);

    TransactionPayload payload1(std::make_shared<EntryFunction>(entryFunction1));

    // Create another identical TransactionPayload object
    EntryFunction entryFunction2 = EntryFunction::Natural(moduleId, "transfer", typeTags, txnArgs);
    TransactionPayload payload2(std::make_shared<EntryFunction>(entryFunction2));

    // Given the same input, GetHashCode should output the same hash
    EXPECT_EQ(payload1.GetHashCode(), payload2.GetHashCode());
}

TEST(RawTransactionSerializeTest, RawTransaction_TransferCoin_Serialize)
{
    TagSequence typeTags{std::vector<std::shared_ptr<ISerializableTag>>{std::make_shared<StructTag>(AccountAddress::FromHex("0x1"), "aptos_coin", "AptosCoin", std::vector<std::shared_ptr<ISerializableTag>>{})}};

    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<AccountAddress>(AccountAddress::FromHex("0x1")),
        std::make_shared<U64>(1000),
    };
    Sequence txnArgs(args);

    ModuleId moduleId(AccountAddress::FromHex("0x1"), "coin");
    EntryFunction payload = EntryFunction::Natural(moduleId, "transfer", typeTags, txnArgs);

    TransactionPayload txnPayload(std::make_shared<EntryFunction>(payload));

    AccountAddress accountAddress = AccountAddress::FromHex("0x1");

    RawTransaction rawTransaction(accountAddress, 0, txnPayload, 2000, 0, 18446744073709551615UL, 4);

    Serialization ser;
    rawTransaction.Serialize(ser);
    std::vector<uint8_t> actual = ser.GetBytes();

    std::vector<uint8_t> expected = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 99, 111, 105, 110, 8, 116, 114, 97, 110, 115, 102, 101, 114, 1, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 10, 97, 112, 116, 111, 115, 95, 99, 111, 105, 110, 9, 65, 112, 116, 111, 115, 67, 111, 105, 110, 0, 2, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8, 232, 3, 0, 0, 0, 0, 0, 0, 208, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 4};

    ASSERT_EQ(expected, actual);
    Deserialization deser(actual);
    auto actualRawTxn = std::dynamic_pointer_cast<RawTransaction>(RawTransaction::Deserialize(deser));

    ASSERT_EQ(rawTransaction, *actualRawTxn);
}

TEST(RawTransactionPrehashTest, RawTransaction_TransferCoin_Prehash)
{
    TagSequence typeTags{std::vector<std::shared_ptr<ISerializableTag>>{std::make_shared<StructTag>(AccountAddress::FromHex("0x1"), "aptos_coin", "AptosCoin", std::vector<std::shared_ptr<ISerializableTag>>{})}};

    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<AccountAddress>(AccountAddress::FromHex("0x1")),
        std::make_shared<U64>(1000),
    };
    Sequence txnArgs(args);

    ModuleId moduleId(AccountAddress::FromHex("0x1"), "coin");
    EntryFunction payload = EntryFunction::Natural(moduleId, "transfer", typeTags, txnArgs);

    TransactionPayload txnPayload(std::make_shared<EntryFunction>(payload));
    AccountAddress accountAddress = AccountAddress::FromHex("0x1");
    RawTransaction rawTransaction(accountAddress, 0, txnPayload, 2000, 0, 18446744073709551615UL, 4);
    std::vector<uint8_t> actual = rawTransaction.Prehash();
    std::vector<uint8_t> expected = {
        181, 233, 125, 176, 127, 160, 189, 14, 85,
        152, 170, 54, 67, 169, 188, 111, 102, 147,
        189, 220, 26, 159, 236, 158, 103, 74, 70,
        30, 170, 0, 177, 147};

    ASSERT_EQ(expected, actual);
}

TEST(RawTransactionKeyedTest, RawTransaction_TransferCoin_Keyed)
{
    TagSequence typeTags{std::vector<std::shared_ptr<ISerializableTag>>{std::make_shared<StructTag>(AccountAddress::FromHex("0x1"), "aptos_coin", "AptosCoin", std::vector<std::shared_ptr<ISerializableTag>>{})}};

    std::vector<std::shared_ptr<ISerializable>> args = {
        std::make_shared<AccountAddress>(AccountAddress::FromHex("0x1")),
        std::make_shared<U64>(1000),
    };
    Sequence txnArgs(args);

    ModuleId moduleId(AccountAddress::FromHex("0x1"), "coin");
    EntryFunction payload = EntryFunction::Natural(moduleId, "transfer", typeTags, txnArgs);
    TransactionPayload txnPayload(std::make_shared<EntryFunction>(payload));
    AccountAddress accountAddress = AccountAddress::FromHex("0x1");
    RawTransaction rawTransaction(accountAddress, 0, txnPayload, 2000, 0, 18446744073709551615UL, 4);
    std::vector<uint8_t> actual = rawTransaction.Keyed();
    std::vector<uint8_t> expected = {181, 233, 125, 176, 127, 160, 189, 14, 85, 152, 170, 54, 67, 169, 188, 111, 102, 147, 189, 220, 26, 159, 236, 158, 103, 74, 70, 30, 170, 0, 177, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 99, 111, 105, 110, 8, 116, 114, 97, 110, 115, 102, 101, 114, 1, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 10, 97, 112, 116, 111, 115, 95, 99, 111, 105, 110, 9, 65, 112, 116, 111, 115, 67, 111, 105, 110, 0, 2, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8, 232, 3, 0, 0, 0, 0, 0, 0, 208, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 4};

    ASSERT_EQ(expected, actual);
}

TEST(EntryFunctionWithCorpusTest, EntryFunction_WithCorpus_Serialize)
{
    std::string senderKeyInput = "9bf49a6a0755f953811fce125f2683d50429c3bb49e074147e0089a52eae155f";
    std::string receiverKeyInput = "0564f879d27ae3c02ce82834acfa8c793a629f2ca0de6919610be82f411326be";
    int sequenceNumberInput = 11;
    int gasUnitPriceInput = 1;
    int maxGasAmountInput = 2000;
    uint64_t expirationTimestampsSecsInput = 1234567890;
    int chainIdInput = 4;
    int amountInput = 5000;

    // Initialize crypto keys and addresses
    PrivateKey senderPrivateKey = PrivateKey::FromHex(senderKeyInput);
    PublicKey senderPublicKey = senderPrivateKey.GetPublicKey();
    AccountAddress senderAccountAddress = AccountAddress::FromKey(senderPublicKey);

    PrivateKey receiverPrivateKey = PrivateKey::FromHex(receiverKeyInput);
    PublicKey receiverPublicKey = receiverPrivateKey.GetPublicKey();
    AccountAddress receiverAccountAddress = AccountAddress::FromKey(receiverPublicKey);

    // Transaction arguments
    std::vector<std::shared_ptr<ISerializable>> txnArgs = {
        std::make_shared<AccountAddress>(receiverAccountAddress),
        std::make_shared<U64>(static_cast<uint64_t>(amountInput))};
    Sequence txnArgsSeq(txnArgs);

    TagSequence typeTags{std::vector<std::shared_ptr<ISerializableTag>>{std::make_shared<StructTag>(AccountAddress::FromHex("0x1"), "aptos_coin", "AptosCoin", std::vector<std::shared_ptr<ISerializableTag>>{})}};

    ModuleId moduleId(AccountAddress::FromHex("0x1"), "coin");
    EntryFunction payload = EntryFunction::Natural(moduleId, "transfer", typeTags, txnArgsSeq);
    TransactionPayload txnPayload(std::make_shared<EntryFunction>(payload));

    // Define RawTransaction
    RawTransaction rawTransactionGenerated(
        senderAccountAddress,
        sequenceNumberInput,
        txnPayload,
        maxGasAmountInput,
        gasUnitPriceInput,
        expirationTimestampsSecsInput,
        chainIdInput);

    // Sign the RawTransaction
    Ed25519Signature senderSignature = rawTransactionGenerated.Sign(senderPrivateKey);
    bool verifySenderSignature = rawTransactionGenerated.Verify(senderPublicKey, senderSignature);
    ASSERT_TRUE(verifySenderSignature);

    // Create an Authenticator
    Authenticator authenticator(std::make_shared<Ed25519Authenticator>(senderPublicKey, senderSignature));

    // Serialize the Authenticator
    Serialization ser;
    authenticator.Serialize(ser);

    // Get the serialized bytes
    std::vector<uint8_t> actual = ser.GetBytes();
    std::vector<uint8_t> expected = {0, 32, 185, 198, 238, 22, 48, 239, 62, 113,
                                     17, 68, 166, 72, 219, 6, 187, 178, 40, 79, 114,
                                     116, 207, 190, 229, 63, 252, 238, 80, 60, 193, 164,
                                     146, 0, 64, 242, 91, 116, 236, 96, 163, 138, 30, 215,
                                     128, 253, 43, 239, 109, 219, 110, 180, 53, 110, 58,
                                     179, 146, 118, 201, 23, 108, 223, 15, 202, 226,
                                     171, 55, 215, 155, 98, 106, 187, 67, 217, 38,
                                     233, 21, 149, 182, 101, 3, 164, 163, 201, 10,
                                     203, 174, 54, 162, 141, 64, 94, 48, 143, 53,
                                     55, 175, 114, 11};

    ASSERT_EQ(expected, actual);

    Deserialization deser(actual);
    auto authenticatorDeserialized = std::dynamic_pointer_cast<Authenticator>(Authenticator::Deserialize(deser));
    ASSERT_EQ(authenticator.GetVariant(), authenticatorDeserialized->GetVariant());
    ASSERT_EQ(authenticator, *authenticatorDeserialized);
    SignedTransaction signedTransactionGenerated(rawTransactionGenerated, authenticator);
    ASSERT_TRUE(signedTransactionGenerated.Verify());
}

TEST(EntryFunction_MultiAgentWithCorpusTest, EntryFunction_MultiAgentWithCorpus_Serialize)
{
    std::string senderKeyInput = "9bf49a6a0755f953811fce125f2683d50429c3bb49e074147e0089a52eae155f";
    std::string receiverKeyInput = "0564f879d27ae3c02ce82834acfa8c793a629f2ca0de6919610be82f411326be";

    int sequenceNumberInput = 11;
    int gasUnitPriceInput = 1;
    int maxGasAmountInput = 2000;
    unsigned long long expirationTimestampsSecsInput = 1234567890;
    int chainIdInput = 4;

    PrivateKey senderPrivateKey = PrivateKey::FromHex(senderKeyInput);
    PublicKey senderPublicKey = senderPrivateKey.GetPublicKey();
    AccountAddress senderAccountAddress = AccountAddress::FromKey(senderPublicKey);

    PrivateKey receiverPrivateKey = PrivateKey::FromHex(receiverKeyInput);
    PublicKey receiverPublicKey = receiverPrivateKey.GetPublicKey();
    AccountAddress receiverAccountAddress = AccountAddress::FromKey(receiverPublicKey);

    std::vector<std::shared_ptr<ISerializable>> txnArgs = {
        std::make_shared<AccountAddress>(receiverAccountAddress),
        std::make_shared<BString>("collection_name"),
        std::make_shared<BString>("token_name"),
        std::make_shared<U64>(1)};
    Sequence txnArgsSeq(txnArgs);
    TagSequence typeTags{std::vector<std::shared_ptr<ISerializableTag>>{}};
    ModuleId moduleId(AccountAddress::FromHex("0x3"), "token");
    EntryFunction payload = EntryFunction::Natural(moduleId, "direct_transfer_script", typeTags, txnArgsSeq);
    TransactionPayload pl(std::make_shared<EntryFunction>(payload));
    RawTransaction rawTxn{
        senderAccountAddress,
        sequenceNumberInput,
        pl,
        maxGasAmountInput,
        gasUnitPriceInput,
        expirationTimestampsSecsInput,
        chainIdInput};
    Sequence sq{std::vector<std::shared_ptr<ISerializable>>{std::make_shared<AccountAddress>(receiverAccountAddress)}};
    MultiAgentRawTransaction rawTransactionGenerated{
        rawTxn,
        sq};

    std::vector<uint8_t> keyedActual = rawTransactionGenerated.Keyed();
    std::vector<uint8_t> keyedExpected = {94, 250, 60, 79, 2, 248, 58, 15, 75, 45, 105, 252, 149, 198, 7, 204, 2, 130, 92, 196, 231, 190, 83, 110, 240, 153, 45, 240, 80, 217, 230, 124, 0, 125, 238, 204, 177, 8, 8, 84, 244, 153, 236, 139, 76, 27, 33, 59, 130, 197, 227, 75, 146, 92, 246, 135, 95, 236, 2, 212, 183, 122, 219, 210, 214, 11, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 5, 116, 111, 107, 101, 110, 22, 100, 105, 114, 101, 99, 116, 95, 116, 114, 97, 110, 115, 102, 101, 114, 95, 115, 99, 114, 105, 112, 116, 0, 4, 32, 45, 19, 61, 221, 40, 27, 182, 32, 85, 88, 53, 124, 198, 172, 117, 102, 24, 23, 233, 170, 234, 195, 175, 235, 195, 40, 66, 117, 156, 191, 127, 169, 16, 15, 99, 111, 108, 108, 101, 99, 116, 105, 111, 110, 95, 110, 97, 109, 101, 11, 10, 116, 111, 107, 101, 110, 95, 110, 97, 109, 101, 8, 1, 0, 0, 0, 0, 0, 0, 0, 208, 7, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 210, 2, 150, 73, 0, 0, 0, 0, 4, 1, 45, 19, 61, 221, 40, 27, 182, 32, 85, 88, 53, 124, 198, 172, 117, 102, 24, 23, 233, 170, 234, 195, 175, 235, 195, 40, 66, 117, 156, 191, 127, 169};

    EXPECT_EQ(keyedExpected, keyedActual);
    Ed25519Signature senderSignature = rawTransactionGenerated.Sign(senderPrivateKey);
    Ed25519Signature receiverSignature = rawTransactionGenerated.Sign(receiverPrivateKey);

    bool verifySenderSignature = rawTransactionGenerated.Verify(senderPublicKey, senderSignature);
    EXPECT_TRUE(verifySenderSignature);
    bool verifyRecieverSignature = rawTransactionGenerated.Verify(receiverPublicKey, receiverSignature);
    EXPECT_TRUE(verifyRecieverSignature);

    Authenticator ed25519AuthSender(std::make_shared<Ed25519Authenticator>(senderPublicKey, senderSignature));

    std::unique_ptr<Serialization> ser = std::make_unique<Serialization>();
    ed25519AuthSender.Serialize(*ser);
    std::vector<uint8_t> actualEd25519Sender = ser->GetBytes();
    std::vector<uint8_t> expectedEd25519Sender = {0, 32, 185, 198, 238, 22, 48, 239, 62,
                                                  113, 17, 68, 166, 72, 219, 6, 187, 178,
                                                  40, 79, 114, 116, 207, 190, 229, 63, 252,
                                                  238, 80, 60, 193, 164, 146, 0, 64, 52, 62,
                                                  123, 16, 170, 50, 60, 72, 3, 145, 165, 215, 205,
                                                  45, 12, 247, 8, 213, 21, 41, 185, 107, 90, 43, 224,
                                                  140, 187, 54, 94, 79, 17, 220, 194, 207, 6, 85, 118,
                                                  108, 247, 13, 64, 133, 59, 156, 57, 91, 98, 218, 215,
                                                  169, 245, 142, 217, 152, 128, 61, 139, 241, 144, 27,
                                                  167, 167, 164, 1};
    EXPECT_EQ(expectedEd25519Sender, actualEd25519Sender);

    // Test ED25519 Authenticator for receiver
    Authenticator ed25519AuthReceiver(std::make_shared<Ed25519Authenticator>(receiverPublicKey, receiverSignature));

    ser = std::make_unique<Serialization>();
    ed25519AuthReceiver.Serialize(*ser);
    std::vector<uint8_t> actualEd25519AuthReceiver = ser->GetBytes();
    std::vector<uint8_t> expectedEd25519AuthReceiver = {0, 32, 174, 243, 244, 164, 184, 236, 161,
                                                        223, 195, 67, 54, 27, 248, 228, 54, 189, 66, 222,
                                                        146, 89, 192, 75, 131, 20, 235, 142, 32, 84, 221,
                                                        110, 130, 171, 64, 138, 127, 6, 228, 4, 174, 141,
                                                        149, 53, 176, 203, 190, 175, 183, 201, 227, 78, 149,
                                                        254, 20, 37, 228, 82, 151, 88, 21, 10, 79, 124, 231,
                                                        166, 131, 53, 65, 72, 173, 92, 49, 62, 195, 101, 73,
                                                        227, 251, 41, 230, 105, 217, 0, 16, 249, 116, 103,
                                                        201, 7, 79, 240, 174, 195, 237, 135, 247, 102, 8};
    EXPECT_EQ(expectedEd25519AuthReceiver, actualEd25519AuthReceiver);

    std::vector<std::tuple<std::shared_ptr<AccountAddress>, std::shared_ptr<Authenticator>>> secondarySignersTup;
    secondarySignersTup.push_back(std::tuple<std::shared_ptr<AccountAddress>, std::shared_ptr<Authenticator>>(
        std::make_shared<AccountAddress>(receiverAccountAddress), std::make_shared<Authenticator>(ed25519AuthReceiver)));
    MultiAgentAuthenticator multiAgentAuthenticator(ed25519AuthSender, secondarySignersTup);
    ser = std::make_unique<Serialization>();
    multiAgentAuthenticator.Serialize(*ser);
    std::vector<uint8_t> actualMultiAgentAuthenticator = ser->GetBytes();
    std::vector<uint8_t> expectedMultiAgentAuthenticator = {0, 32, 185, 198, 238, 22, 48, 239, 62, 113, 17, 68,
                                                            166, 72, 219, 6, 187, 178, 40, 79, 114, 116, 207, 190,
                                                            229, 63, 252, 238, 80, 60, 193, 164, 146, 0, 64, 52,
                                                            62, 123, 16, 170, 50, 60, 72, 3, 145, 165, 215, 205,
                                                            45, 12, 247, 8, 213, 21, 41, 185, 107, 90, 43, 224,
                                                            140, 187, 54, 94, 79, 17, 220, 194, 207, 6, 85, 118,
                                                            108, 247, 13, 64, 133, 59, 156, 57, 91, 98, 218, 215,
                                                            169, 245, 142, 217, 152, 128, 61, 139, 241, 144, 27,
                                                            167, 167, 164, 1, 1, 45, 19, 61, 221, 40, 27, 182,
                                                            32, 85, 88, 53, 124, 198, 172, 117, 102, 24, 23,
                                                            233, 170, 234, 195, 175, 235, 195, 40, 66, 117,
                                                            156, 191, 127, 169, 1, 0, 32, 174, 243, 244, 164,
                                                            184, 236, 161, 223, 195, 67, 54, 27, 248, 228, 54,
                                                            189, 66, 222, 146, 89, 192, 75, 131, 20, 235, 142,
                                                            32, 84, 221, 110, 130, 171, 64, 138, 127, 6, 228,
                                                            4, 174, 141, 149, 53, 176, 203, 190, 175, 183, 201,
                                                            227, 78, 149, 254, 20, 37, 228, 82, 151, 88, 21, 10,
                                                            79, 124, 231, 166, 131, 53, 65, 72, 173, 92, 49, 62,
                                                            195, 101, 73, 227, 251, 41, 230, 105, 217, 0, 16, 249,
                                                            116, 103, 201, 7, 79, 240, 174, 195, 237, 135, 247, 102, 8};
    EXPECT_EQ(actualMultiAgentAuthenticator, expectedMultiAgentAuthenticator);

    Authenticator authenticator(std::make_shared<MultiAgentAuthenticator>(multiAgentAuthenticator));
    ser = std::make_unique<Serialization>();
    authenticator.Serialize(*ser);
    std::vector<uint8_t> actualAuthenticator = ser->GetBytes();
    std::vector<uint8_t> expectedAuthenticator = {2, 0, 32, 185, 198, 238, 22, 48, 239, 62, 113, 17, 68, 166,
                                                  72, 219, 6, 187, 178, 40, 79, 114, 116, 207, 190, 229, 63,
                                                  252, 238, 80, 60, 193, 164, 146, 0, 64, 52, 62, 123, 16,
                                                  170, 50, 60, 72, 3, 145, 165, 215, 205, 45, 12, 247, 8,
                                                  213, 21, 41, 185, 107, 90, 43, 224, 140, 187, 54, 94,
                                                  79, 17, 220, 194, 207, 6, 85, 118, 108, 247, 13, 64,
                                                  133, 59, 156, 57, 91, 98, 218, 215, 169, 245, 142, 217,
                                                  152, 128, 61, 139, 241, 144, 27, 167, 167, 164, 1, 1, 45,
                                                  19, 61, 221, 40, 27, 182, 32, 85, 88, 53, 124, 198, 172,
                                                  117, 102, 24, 23, 233, 170, 234, 195, 175, 235, 195,
                                                  40, 66, 117, 156, 191, 127, 169, 1, 0, 32, 174, 243,
                                                  244, 164, 184, 236, 161, 223, 195, 67, 54, 27, 248,
                                                  228, 54, 189, 66, 222, 146, 89, 192, 75, 131, 20, 235,
                                                  142, 32, 84, 221, 110, 130, 171, 64, 138, 127, 6, 228,
                                                  4, 174, 141, 149, 53, 176, 203, 190, 175, 183, 201, 227,
                                                  78, 149, 254, 20, 37, 228, 82, 151, 88, 21, 10, 79, 124,
                                                  231, 166, 131, 53, 65, 72, 173, 92, 49, 62, 195, 101, 73,
                                                  227, 251, 41, 230, 105, 217, 0, 16, 249, 116, 103, 201, 7,
                                                  79, 240, 174, 195, 237, 135, 247, 102, 8};
    EXPECT_EQ(actualAuthenticator, expectedAuthenticator);

    Deserialization deser(actualAuthenticator);
    auto authenticatorDeserialized = std::dynamic_pointer_cast<Authenticator>(Authenticator::Deserialize(deser));
    EXPECT_EQ(authenticator.GetVariant(), authenticatorDeserialized->GetVariant());
    EXPECT_EQ(authenticator, *authenticatorDeserialized);

    SignedTransaction signedTransactionGenerated(rawTransactionGenerated.Inner(), authenticator);

    ser = std::make_unique<Serialization>();
    signedTransactionGenerated.Serialize(*ser);
    std::vector<uint8_t> signedTxnActual = ser->GetBytes();
    std::vector<uint8_t> signedTxnExpected = {125, 238, 204, 177, 8, 8, 84, 244, 153, 236, 139, 76, 27,
                                              33, 59, 130, 197, 227, 75, 146, 92, 246, 135, 95, 236, 2,
                                              212, 183, 122, 219, 210, 214, 11, 0, 0, 0, 0, 0, 0, 0, 2,
                                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 5, 116, 111, 107, 101,
                                              110, 22, 100, 105, 114, 101, 99, 116, 95, 116, 114, 97, 110,
                                              115, 102, 101, 114, 95, 115, 99, 114, 105, 112, 116, 0, 4, 32,
                                              45, 19, 61, 221, 40, 27, 182, 32, 85, 88, 53, 124, 198, 172,
                                              117, 102, 24, 23, 233, 170, 234, 195, 175, 235, 195, 40, 66,
                                              117, 156, 191, 127, 169, 16, 15, 99, 111, 108, 108, 101, 99,
                                              116, 105, 111, 110, 95, 110, 97, 109, 101, 11, 10, 116, 111,
                                              107, 101, 110, 95, 110, 97, 109, 101, 8, 1, 0, 0, 0, 0, 0,
                                              0, 0, 208, 7, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 210,
                                              2, 150, 73, 0, 0, 0, 0, 4, 2, 0, 32, 185, 198, 238, 22, 48,
                                              239, 62, 113, 17, 68, 166, 72, 219, 6, 187, 178, 40, 79, 114,
                                              116, 207, 190, 229, 63, 252, 238, 80, 60, 193, 164, 146, 0, 64,
                                              52, 62, 123, 16, 170, 50, 60, 72, 3, 145, 165, 215, 205, 45, 12,
                                              247, 8, 213, 21, 41, 185, 107, 90, 43, 224, 140, 187, 54, 94, 79,
                                              17, 220, 194, 207, 6, 85, 118, 108, 247, 13, 64, 133, 59, 156, 57,
                                              91, 98, 218, 215, 169, 245, 142, 217, 152, 128, 61, 139, 241, 144,
                                              27, 167, 167, 164, 1, 1, 45, 19, 61, 221, 40, 27, 182, 32, 85, 88,
                                              53, 124, 198, 172, 117, 102, 24, 23, 233, 170, 234, 195, 175, 235,
                                              195, 40, 66, 117, 156, 191, 127, 169, 1, 0, 32, 174, 243, 244, 164,
                                              184, 236, 161, 223, 195, 67, 54, 27, 248, 228, 54, 189, 66, 222, 146,
                                              89, 192, 75, 131, 20, 235, 142, 32, 84, 221, 110, 130, 171, 64, 138,
                                              127, 6, 228, 4, 174, 141, 149, 53, 176, 203, 190, 175, 183, 201, 227,
                                              78, 149, 254, 20, 37, 228, 82, 151, 88, 21, 10, 79, 124, 231, 166,
                                              131, 53, 65, 72, 173, 92, 49, 62, 195, 101, 73, 227, 251, 41, 230,
                                              105, 217, 0, 16, 249, 116, 103, 201, 7, 79, 240, 174, 195, 237, 135,
                                              247, 102, 8};
    EXPECT_EQ(signedTxnExpected, signedTxnActual);

    EXPECT_TRUE(signedTransactionGenerated.Verify());

    std::string rawTransactionInput = "7deeccb1080854f499ec8b4c1b213b82c5e34b925cf6875fec02d4b77adbd2d60b0000000000000002000000000000000000000000000000000000000000000000000000000000000305746f6b656e166469726563745f7472616e736665725f7363726970740004202d133ddd281bb6205558357cc6ac75661817e9aaeac3afebc32842759cbf7fa9100f636f6c6c656374696f6e5f6e616d650b0a746f6b656e5f6e616d65080100000000000000d0070000000000000100000000000000d20296490000000004";
    std::string signedTransactionInput = "7deeccb1080854f499ec8b4c1b213b82c5e34b925cf6875fec02d4b77adbd2d60b0000000000000002000000000000000000000000000000000000000000000000000000000000000305746f6b656e166469726563745f7472616e736665725f7363726970740004202d133ddd281bb6205558357cc6ac75661817e9aaeac3afebc32842759cbf7fa9100f636f6c6c656374696f6e5f6e616d650b0a746f6b656e5f6e616d65080100000000000000d0070000000000000100000000000000d20296490000000004020020b9c6ee1630ef3e711144a648db06bbb2284f7274cfbee53ffcee503cc1a4920040343e7b10aa323c480391a5d7cd2d0cf708d51529b96b5a2be08cbb365e4f11dcc2cf0655766cf70d40853b9c395b62dad7a9f58ed998803d8bf1901ba7a7a401012d133ddd281bb6205558357cc6ac75661817e9aaeac3afebc32842759cbf7fa9010020aef3f4a4b8eca1dfc343361bf8e436bd42de9259c04b8314eb8e2054dd6e82ab408a7f06e404ae8d9535b0cbbeafb7c9e34e95fe1425e4529758150a4f7ce7a683354148ad5c313ec36549e3fb29e669d90010f97467c9074ff0aec3ed87f76608";

    // Produce serialized generated transactions
    ser = std::make_unique<Serialization>();
    ser->Serialize(rawTransactionGenerated.Inner());
    std::string rawTransactionGeneratedBytes = Utils::HexStringFromByteArray(ser->GetBytes());

    ser = std::make_unique<Serialization>();
    ser->Serialize(signedTransactionGenerated);
    std::string signedTransactionGeneratedBytes = Utils::HexStringFromByteArray(ser->GetBytes());

    // Verify the RawTransaction
    EXPECT_EQ(rawTransactionInput, rawTransactionGeneratedBytes);
    Deserialization d(Utils::ByteArrayFromHexString(rawTransactionInput));
    auto rawTransaction = std::dynamic_pointer_cast<RawTransaction>(RawTransaction::Deserialize(d));
    EXPECT_EQ(rawTransactionGenerated.Inner(), *rawTransaction);

    // Verify the SignedTransaction
    EXPECT_EQ(signedTransactionInput, signedTransactionGeneratedBytes);
    Deserialization d2(Utils::ByteArrayFromHexString(signedTransactionInput));

    auto signedTransaction = std::dynamic_pointer_cast<SignedTransaction>(SignedTransaction::Deserialize(d2));

    EXPECT_EQ(signedTransaction->getTransaction(), *rawTransaction);
    EXPECT_EQ(*signedTransaction, signedTransactionGenerated);
    EXPECT_TRUE(signedTransaction->Verify());
    EXPECT_TRUE(signedTransaction->Bytes().size() > 0);
    EXPECT_EQ(signedTransaction->ToString(), "Transaction: RawTransaction\n Authenticator: MultiAgentAuthenticator");
    EXPECT_TRUE(signedTransaction->GetHashCode() != 0);
}

// Unitest

TEST(ScriptArgumentTest, U8Type)
{
    auto scriptArg = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::U8, std::make_shared<U8>(100));
    Serialization ser;
    scriptArg->Serialize(ser);

    Deserialization des(ser.GetBytes());

    std::shared_ptr<ScriptArgument> result = std::dynamic_pointer_cast<ScriptArgument>(ScriptArgument::Deserialize(des));

    ASSERT_TRUE(scriptArg->Equals(*result));
}

TEST(ScriptArgumentTest, U16Type)
{
    auto scriptArg = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::U16, std::make_shared<U16>(100));
    Serialization ser;
    scriptArg->Serialize(ser);

    Deserialization des(ser.GetBytes());

    std::shared_ptr<ScriptArgument> result = std::dynamic_pointer_cast<ScriptArgument>(ScriptArgument::Deserialize(des));

    ASSERT_TRUE(scriptArg->Equals(*result));
}

TEST(ScriptArgumentTest, U32Type)
{
    auto scriptArg = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::U32, std::make_shared<U32>(100));
    Serialization ser;
    scriptArg->Serialize(ser);

    Deserialization des(ser.GetBytes());

    std::shared_ptr<ScriptArgument> result = std::dynamic_pointer_cast<ScriptArgument>(ScriptArgument::Deserialize(des));

    ASSERT_TRUE(scriptArg->Equals(*result));
}

TEST(ScriptArgumentTest, U128Type)
{
    auto scriptArg = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::U128, std::make_shared<U128>(100));
    Serialization ser;
    scriptArg->Serialize(ser);

    Deserialization des(ser.GetBytes());

    std::shared_ptr<ScriptArgument> result = std::dynamic_pointer_cast<ScriptArgument>(ScriptArgument::Deserialize(des));

    ASSERT_TRUE(scriptArg->Equals(*result));
}

TEST(ScriptArgumentTest, U256Type)
{
    auto scriptArg = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::U256, std::make_shared<U256>(100));
    Serialization ser;
    scriptArg->Serialize(ser);

    Deserialization des(ser.GetBytes());

    std::shared_ptr<ScriptArgument> result = std::dynamic_pointer_cast<ScriptArgument>(ScriptArgument::Deserialize(des));

    ASSERT_TRUE(scriptArg->Equals(*result));
}

TEST(ScriptArgumentTest, BoolType)
{
    auto scriptArg = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::BOOL, std::make_shared<Bool>(true));
    Serialization ser;
    scriptArg->Serialize(ser);

    Deserialization des(ser.GetBytes());

    std::shared_ptr<ScriptArgument> result = std::dynamic_pointer_cast<ScriptArgument>(ScriptArgument::Deserialize(des));

    ASSERT_TRUE(scriptArg->Equals(*result));
}

TEST(ScriptArgumentTest, ADDRESSType)
{
    std::string senderKeyInput = "9bf49a6a0755f953811fce125f2683d50429c3bb49e074147e0089a52eae155f";
    PrivateKey senderPrivateKey = PrivateKey::FromHex(senderKeyInput);
    PublicKey senderPublicKey = senderPrivateKey.GetPublicKey();
    AccountAddress senderAccountAddress = AccountAddress::FromKey(senderPublicKey);
    auto scriptArg = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::ACCOUNT_ADDRESS, std::make_shared<AccountAddress>(senderAccountAddress));
    Serialization ser;
    scriptArg->Serialize(ser);

    Deserialization des(ser.GetBytes());

    std::shared_ptr<ScriptArgument> result = std::dynamic_pointer_cast<ScriptArgument>(ScriptArgument::Deserialize(des));

    ASSERT_TRUE(scriptArg->Equals(*result));
}

TEST(ScriptArgumentTest, ToString)
{
    std::string senderKeyInput = "9bf49a6a0755f953811fce125f2683d50429c3bb49e074147e0089a52eae155f";
    PrivateKey senderPrivateKey = PrivateKey::FromHex(senderKeyInput);
    PublicKey senderPublicKey = senderPrivateKey.GetPublicKey();
    AccountAddress senderAccountAddress = AccountAddress::FromKey(senderPublicKey);
    auto scriptArg = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::ACCOUNT_ADDRESS, std::make_shared<AccountAddress>(senderAccountAddress));

    ASSERT_EQ(scriptArg->ToString(), "[3] 0x7deeccb1080854f499ec8b4c1b213b82c5e34b925cf6875fec02d4b77adbd2d6");
}

TEST(ScriptArgumentTest, getHashCode)
{
    std::string senderKeyInput = "9bf49a6a0755f953811fce125f2683d50429c3bb49e074147e0089a52eae155f";
    PrivateKey senderPrivateKey = PrivateKey::FromHex(senderKeyInput);
    PublicKey senderPublicKey = senderPrivateKey.GetPublicKey();
    AccountAddress senderAccountAddress = AccountAddress::FromKey(senderPublicKey);
    auto scriptArg = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::ACCOUNT_ADDRESS, std::make_shared<AccountAddress>(senderAccountAddress));
    auto scriptArg2 = std::make_shared<ScriptArgument>(ScriptArgumentTypeTag::BOOL, std::make_shared<Bool>(true));

    ASSERT_NE(scriptArg->GetHashCode(), scriptArg2->GetHashCode());
}