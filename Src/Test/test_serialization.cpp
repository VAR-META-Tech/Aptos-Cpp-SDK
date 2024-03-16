// test_serialization.cpp

#include <gtest/gtest.h>
#include "../BCS/BCS.h"
#include <cryptopp/integer.h>
#include "../BCS/StructTag.h"
#include "../HDWallet/Utils/Utils.h"

using namespace Aptos::BCS;
using namespace Aptos::Accounts;
using namespace Aptos::Accounts::Ed25519;

TEST(SerializationTest, BoolSerialize)
{
    Serialization s;
    s.SerializeBool(true);
    std::vector<uint8_t> res = s.GetBytes();

    std::vector<uint8_t> expected = {1};
    ASSERT_EQ(expected, res);
}

TEST(SerializationTest, BoolTrueDeserialize)
{
    bool actual = true;
    Serialization s;
    s.SerializeBool(actual);
    std::vector<uint8_t> res = s.GetBytes();

    Deserialization d(res);
    bool expected = d.DeserializeBool();

    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, BoolFalseSerialize)
{
    Serialization s;
    s.SerializeBool(false);
    std::vector<uint8_t> res = s.GetBytes();

    std::vector<uint8_t> expected = {0};
    ASSERT_EQ(expected, res);
}

TEST(SerializationTest, BoolFalseDeserialize)
{
    bool expected = false;
    Serialization s;
    s.SerializeBool(expected);
    std::vector<uint8_t> res = s.GetBytes();

    Deserialization d(res);
    bool actual = d.DeserializeBool();

    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, ByteArraySerialize)
{

    std::string value = "1234567890";

    std::vector<uint8_t> bytes(value.begin(), value.end());

    Serialization s;
    s.SerializeBytes(bytes);

    std::vector<uint8_t> actual = s.GetBytes();

    std::vector<uint8_t> expected = {10, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, ByteArrayDeserialize)
{
    std::string value = "1234567890";

    std::vector<uint8_t> bytes(value.begin(), value.end());

    Serialization s;
    s.SerializeBytes(bytes);
    std::vector<uint8_t> expected = s.GetBytes();

    Deserialization d(expected);
    std::vector<uint8_t> actual = d.ToBytes();

    ASSERT_EQ(value.size(), actual.size());
    ASSERT_EQ(std::vector<uint8_t>(value.begin(), value.end()), actual);
}

TEST(SerializationTest, U8Serialize)
{

    Serialization s;
    s.SerializeU8(123);

    std::vector<uint8_t> expected = {123};
    auto actual = s.GetBytes();

    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, U8Deserialize)
{

    uint8_t expected = 123;

    Serialization s;
    s.SerializeU8(expected);

    std::vector<uint8_t> bytes = s.GetBytes();

    Deserialization d(bytes);
    uint8_t actual = d.DeserializeU8();

    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, U32Serialize)
{

    uint32_t input = 57615782;

    Serialization s;
    s.SerializeU32(input);

    std::vector<uint8_t> expected = {166, 37, 111, 3};
    auto actual = s.GetBytes();

    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, U32Deserialize)
{

    uint32_t expected = 57615782;

    Serialization s;
    s.SerializeU32(expected);

    std::vector<uint8_t> bytes = s.GetBytes();

    Deserialization d(bytes);
    uint32_t actual = d.DeserializeU32();

    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, U64Serialize)
{

    uint64_t input = 9432012321182;

    Serialization s;
    s.SerializeU64(input);

    std::vector<uint8_t> expected = {158, 113, 190, 15, 148, 8, 0, 0};
    auto actual = s.GetBytes();

    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, U64Deserialize)
{

    uint64_t expected = 9432012321182;

    Serialization s;
    s.SerializeU64(expected);

    std::vector<uint8_t> bytes = s.GetBytes();

    Deserialization d(bytes);
    uint64_t actual = d.DeserializeU64();

    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, U128Serialize)
{

    CryptoPP::Integer input1 = 10;

    Serialization s1;
    s1.SerializeU128(input1);

    std::vector<uint8_t> expected1 = {10, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0};

    ASSERT_EQ(expected1, s1.GetBytes());

    CryptoPP::Integer input2 = CryptoPP::Integer("749382032131231323910498053");

    Serialization s2;
    s2.SerializeU128(input2);

    std::vector<uint8_t> expected2 = {5, 231, 86, 201, 40, 241, 231, 92,
                                      209, 223, 107, 2, 0, 0, 0, 0};

    ASSERT_EQ(expected2, s2.GetBytes());
}

TEST(SerializationTest, U128Deserialize)
{
    CryptoPP::Integer expected = 10;
    std::vector<uint8_t> bytes = Serialization().SerializeU128(expected).GetBytes();
    CryptoPP::Integer actual = Deserialization(bytes).DeserializeU128();
    ASSERT_EQ(expected, actual);

    expected = CryptoPP::Integer("749382032131231323910498053");
    bytes = Serialization().SerializeU128(expected).GetBytes();
    actual = Deserialization(bytes).DeserializeU128();
    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, U32AsUleb128Serialize)
{
    std::vector<uint8_t> res = Serialization().SerializeU32AsUleb128(1160).GetBytes();
    std::vector<uint8_t> expected = {136, 9};
    ASSERT_EQ(expected, res);
}

TEST(SerializationTest, U32AsUleb128Deserialize)
{
    uint32_t expected = 1160;
    std::vector<uint8_t> bytes = Serialization().SerializeU32AsUleb128(expected).GetBytes();
    uint32_t actual = Deserialization(bytes).DeserializeUleb128();
    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, StringSerialize)
{
    std::vector<uint8_t> res = Serialization().SerializeString("potato UTF8: 🥔").GetBytes();
    std::vector<uint8_t> exp = {17, 112, 111, 116, 97, 116, 111, 32, 85, 84, 70, 56, 58, 32, 240, 159, 165, 148};
    ASSERT_EQ(exp, res);
}

TEST(SerializationTest, StringDeserialize)
{
    std::string expected = "potato UTF8: 🥔";
    std::vector<uint8_t> bytes = Serialization().SerializeString(expected).GetBytes();
    std::string actual = Deserialization(bytes).DeserializeString();
    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, StringLongSerialize)
{
    std::string longString = "potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔";
    std::vector<uint8_t> res = Serialization().SerializeString(longString).GetBytes();

    std::vector<uint8_t> exp = {231, 2, 112, 111, 116, 97, 116, 111, 32, 85, 84, 70,
                                56, 58, 32, 240, 159, 165, 148, 32, 112, 111, 116, 97,
                                116, 111, 32, 85, 84, 70, 56, 58, 32, 240, 159, 165,
                                148, 32, 112, 111, 116, 97, 116, 111, 32, 85, 84, 70,
                                56, 58, 32, 240, 159, 165, 148, 32, 112, 111, 116, 97,
                                116, 111, 32, 85, 84, 70, 56, 58, 32, 240, 159, 165,
                                148, 32, 112, 111, 116, 97, 116, 111, 32, 85, 84, 70,
                                56, 58, 32, 240, 159, 165, 148, 32, 112, 111, 116, 97,
                                116, 111, 32, 85, 84, 70, 56, 58, 32, 240, 159, 165,
                                148, 32, 112, 111, 116, 97, 116, 111, 32, 85, 84, 70,
                                56, 58, 32, 240, 159, 165, 148, 32, 112, 111, 116, 97,
                                116, 111, 32, 85, 84, 70, 56, 58, 32, 240, 159, 165,
                                148, 32, 112, 111, 116, 97, 116, 111, 32, 85, 84, 70,
                                56, 58, 32, 240, 159, 165, 148, 32, 112, 111, 116, 97,
                                116, 111, 32, 85, 84, 70, 56, 58, 32, 240, 159, 165,
                                148, 32, 112, 111, 116, 97, 116, 111, 32, 85, 84, 70,
                                56, 58, 32, 240, 159, 165, 148, 32, 112, 111, 116, 97,
                                116, 111, 32, 85, 84, 70, 56, 58, 32, 240, 159, 165,
                                148, 32, 112, 111, 116, 97, 116, 111, 32, 85, 84, 70,
                                56, 58, 32, 240, 159, 165, 148, 32, 112, 111, 116, 97,
                                116, 111, 32, 85, 84, 70, 56, 58, 32, 240, 159, 165,
                                148, 32, 112, 111, 116, 97, 116, 111, 32, 85, 84, 70,
                                56, 58, 32, 240, 159, 165, 148, 32, 112, 111, 116, 97,
                                116, 111, 32, 85, 84, 70, 56, 58, 32, 240, 159, 165,
                                148, 32, 112, 111, 116, 97, 116, 111, 32, 85, 84, 70,
                                56, 58, 32, 240, 159, 165, 148, 32, 112, 111, 116, 97,
                                116, 111, 32, 85, 84, 70, 56, 58, 32, 240, 159, 165,
                                148, 32, 112, 111, 116, 97, 116, 111, 32, 85, 84, 70,
                                56, 58, 32, 240, 159, 165, 148, 32, 112, 111, 116, 97,
                                116, 111, 32, 85, 84, 70, 56, 58, 32, 240, 159, 165,
                                148};

    for (int i = 0; i < exp.size(); i++)
    {
        ASSERT_EQ(exp[i], res[i]);
    }
}

TEST(SerializationTest, StringLongDeserialize)
{
    std::string expected = "potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔";
    std::vector<uint8_t> bytes = Serialization().SerializeString(expected).GetBytes();
    std::string actual = Deserialization(bytes).DeserializeString();

    ASSERT_EQ(expected, actual);
}

using MapType = std::map<BString, std::shared_ptr<ISerializable>>;

TEST(SerializationTest, MapBStringU32Test)
{
    MapType map;
    map[BString("a")] = std::make_shared<U32>(12345);
    map[BString("b")] = std::make_shared<U32>(99234);
    map[BString("c")] = std::make_shared<U32>(23829);

    BCSMap bcsMap(map);
    Serialization ser;
    bcsMap.Serialize(ser);

    std::vector<uint8_t> serialized = ser.GetBytes();
    std::vector<uint8_t> expected = {3, 1, 97, 57, 48, 0, 0, 1, 98, 162, 131, 1, 0, 1, 99, 21, 93, 0, 0};

    EXPECT_EQ(expected, serialized);
}

TEST(SerializationTest, Map_BString_U32_Deserialize)
{
    MapType expectedMap;
    expectedMap[BString("x")] = std::make_shared<U32>(12345);
    expectedMap[BString("b")] = std::make_shared<U32>(99234);
    expectedMap[BString("c")] = std::make_shared<U32>(23829);

    BCSMap expectedBcsMap(expectedMap);
    Serialization ser;
    expectedBcsMap.Serialize(ser);

    std::vector<uint8_t> bytes = ser.GetBytes();
    Deserialization deser(bytes);
    BCSMap actualBcsMap = deser.DeserializeMap(
        [](Deserialization &d) -> std::shared_ptr<ISerializable>
        { return U32::Deserialize(d); });

    auto values = actualBcsMap.getValues();
    // Expect the keys and values to be the same, but in sorted order
    std::vector<BString> key;
    std::vector<std::shared_ptr<ISerializable>> value;
    for (auto it = values.begin(); it != values.end(); ++it)
    {
        key.push_back(it->first);
        value.push_back(it->second);
    }

    EXPECT_TRUE(BString("b").Equals(key[0]));
    EXPECT_TRUE(BString("c").Equals(key[1]));
    EXPECT_TRUE(BString("x").Equals(key[2]));
    EXPECT_TRUE(U32(99234).Equals(*(std::dynamic_pointer_cast<U32>(value[0]))));
    EXPECT_TRUE(U32(23829).Equals(*(std::dynamic_pointer_cast<U32>(value[1]))));
    EXPECT_TRUE(U32(12345).Equals(*(std::dynamic_pointer_cast<U32>(value[2]))));
}

TEST(SerializationTest, MapBStringU32Test1)
{
    MapType map;
    map[BString("x")] = std::make_shared<U32>(12345);
    map[BString("b")] = std::make_shared<U32>(99234);
    map[BString("c")] = std::make_shared<U32>(23829);

    BCSMap bcsMap(map);
    Serialization ser;
    bcsMap.Serialize(ser);

    std::vector<uint8_t> serialized = ser.GetBytes();
    std::vector<uint8_t> expected = {3, 1, 98, 162, 131, 1, 0, 1, 99, 21, 93, 0, 0, 1, 120, 57, 48, 0, 0};

    EXPECT_EQ(expected, serialized);
}

TEST(SerializationTest, MapBStringU32Test2)
{
    MapType map;
    map[BString("b")] = std::make_shared<U32>(12345);
    map[BString("x")] = std::make_shared<U32>(99234);
    map[BString("c")] = std::make_shared<U32>(23829);

    BCSMap bcsMap(map);
    Serialization ser;
    bcsMap.Serialize(ser);

    std::vector<uint8_t> serialized = ser.GetBytes();
    std::vector<uint8_t> expected = {3, 1, 98, 57, 48, 0, 0, 1, 99, 21, 93, 0, 0, 1, 120, 162, 131, 1, 0};

    EXPECT_EQ(expected, serialized);
}

TEST(SerializationTest, MapBStringU32Test3)
{
    MapType map;
    map[BString("b")] = std::make_shared<U32>(99234);
    map[BString("x")] = std::make_shared<U32>(12345);
    map[BString("c")] = std::make_shared<U32>(23829);

    BCSMap bcsMap(map);
    Serialization ser;
    bcsMap.Serialize(ser);

    std::vector<uint8_t> serialized = ser.GetBytes();
    std::vector<uint8_t> expected = {3, 1, 98, 162, 131, 1, 0, 1, 99, 21, 93, 0, 0, 1, 120, 57, 48, 0, 0};

    EXPECT_EQ(expected, serialized);
}

TEST(SerializationTest, MapBStringBStringTest)
{
    MapType map;
    map[BString("a")] = std::make_shared<BString>("a");
    map[BString("b")] = std::make_shared<BString>("b");
    map[BString("c")] = std::make_shared<BString>("c");

    BCSMap bcsMap(map);
    Serialization ser;
    bcsMap.Serialize(ser);

    std::vector<uint8_t> serialized = ser.GetBytes();
    std::vector<uint8_t> expected = {3, 1, 97, 1, 97, 1, 98, 1, 98, 1, 99, 1, 99};

    EXPECT_EQ(expected, serialized);
}

TEST(SerializationTest, StringBytesEmptySerialize)
{
    std::vector<uint8_t> value(1160, 0); // empty byte string of size 1160

    Serialization serializer;
    serializer.SerializeBytes(value);
    std::vector<uint8_t> res = serializer.GetBytes();

    // 1160 for byte array + 2 for length
    std::vector<uint8_t> exp(1162, 0);
    exp[0] = 136;
    exp[1] = 9;

    EXPECT_EQ(exp, res);
}

TEST(SerializationTest, MultipleValues_Serialize)
{
    Serialization serializer;
    serializer.Serialize(std::string("potato"));
    serializer.Serialize(static_cast<uint32_t>(123));
    serializer.Serialize(true);
    serializer.Serialize(static_cast<uint32_t>(456));
    std::vector<uint8_t> res = serializer.GetBytes();
    std::vector<uint8_t> exp = {6, 112, 111, 116, 97, 116, 111, 123, 0, 0, 0, 1, 200, 1, 0, 0};
    EXPECT_EQ(exp, res);
}

TEST(SerializationTest, SequenceBStringEmptySerialize)
{
    Serialization ser;
    std::vector<std::shared_ptr<ISerializable>> strArr = {std::make_shared<BString>("")};
    Sequence seq(strArr);

    seq.Serialize(ser);

    std::vector<uint8_t> actual = ser.GetBytes();
    std::vector<uint8_t> expected = {1, 1, 0};
    EXPECT_EQ(expected, actual);
}

TEST(SerializationTest, SequenceBStringLongDeserialize)
{
    Serialization ser;
    std::vector<std::shared_ptr<ISerializable>> expectedStrArr = {std::make_shared<BString>("a"), std::make_shared<BString>("abc"),
                                                                  std::make_shared<BString>("def"), std::make_shared<BString>("ghi")};
    ser.Serialize(expectedStrArr);

    Sequence expectedSeq(expectedStrArr);

    std::vector<uint8_t> actual = ser.GetBytes();
    std::vector<uint8_t> exp = {4, 1, 97, 3, 97, 98, 99, 3, 100, 101, 102, 3, 103, 104, 105};
    EXPECT_EQ(exp, actual);

    Deserialization deser(actual);
    std::vector<std::shared_ptr<ISerializable>> actualSequenceArr = deser.DeserializeSequence(
        [](Deserialization &d) -> std::shared_ptr<ISerializable>
        { return BString::Deserialize(d); });

    for (int i = 0; i < expectedStrArr.size(); i++)
    {
        EXPECT_EQ(expectedStrArr[i]->ToString(), actualSequenceArr[i]->ToString());
    }
}

TEST(SerializationTest, SequenceBStringSerialize)
{
    Serialization ser;
    std::vector<std::shared_ptr<ISerializable>> expectedStrArr = {std::make_shared<BString>("a"), std::make_shared<BString>("abc"),
                                                                  std::make_shared<BString>("def"), std::make_shared<BString>("ghi")};
    ser.Serialize(expectedStrArr);

    std::vector<uint8_t> actual = ser.GetBytes();
    std::vector<uint8_t> exp = {4, 1, 97, 3, 97, 98, 99, 3, 100, 101, 102, 3, 103, 104, 105};
    EXPECT_EQ(exp, actual);
}

TEST(SerializationTest, SequenceBoolSerialize)
{
    Serialization ser;
    std::vector<std::shared_ptr<ISerializable>> expectedStrArr = {std::make_shared<Bool>(false), std::make_shared<Bool>(true),
                                                                  std::make_shared<Bool>(false)};
    ser.Serialize(expectedStrArr);

    std::vector<uint8_t> actual = ser.GetBytes();
    std::vector<uint8_t> exp = {3, 0, 1, 0};
    EXPECT_EQ(exp, actual);
}

TEST(SerializationTest, SequenceBoolDeserializer)
{
    Serialization ser;
    std::vector<std::shared_ptr<ISerializable>> expectedBoolArr = {std::make_shared<Bool>(false), std::make_shared<Bool>(true),
                                                                   std::make_shared<Bool>(false)};
    ser.Serialize(expectedBoolArr);

    std::vector<uint8_t> expectedByteArr = {3, 0, 1, 0};
    std::vector<uint8_t> actualByteArr = ser.GetBytes();
    EXPECT_EQ(expectedByteArr, actualByteArr);

    Deserialization deser(expectedByteArr);
    std::vector<std::shared_ptr<ISerializable>> actualBoolArr = deser.DeserializeSequence(
        [](Deserialization &d) -> std::shared_ptr<ISerializable>
        { return Bool::Deserialize(d); });
    EXPECT_EQ(expectedBoolArr.size(), actualBoolArr.size());
    for (std::size_t i = 0; i < expectedBoolArr.size(); ++i)
    {
        EXPECT_EQ(*std::dynamic_pointer_cast<Bool>(expectedBoolArr[i]),
                  *std::dynamic_pointer_cast<Bool>(actualBoolArr[i]));
    }
}

// Addition testcase for Unit Test

TEST(SerializationTest, BCSMapSerialize)
{
    // Instantiate some ISerializable objects to use as values in the BCSMap
    auto boolValue1 = std::make_shared<Bool>(true);
    auto boolValue2 = std::make_shared<Bool>(false);

    // Create a BCSMap
    std::map<BString, std::shared_ptr<ISerializable>> mapValues = {
        {BString("key1"), boolValue1},
        {BString("key2"), boolValue2}};
    BCSMap map(mapValues);

    // Serialize the map
    Serialization mapSerializer;
    map.Serialize(mapSerializer);

    // Get the serialized data
    std::vector<uint8_t> actual = mapSerializer.GetBytes();

    // Expected output
    // The expected output depends on the implementation of BCSMap, BString, and Bool.
    // Modify this to match your expected output.
    std::vector<uint8_t> expected = {
        '\x2', '\x4', 'k', 'e', 'y', '1', '\x1', '\x4', 'k', 'e', 'y', '2', '\0'};

    // Check if the serialized data matches the expected output
    EXPECT_EQ(expected, actual);
}

TEST(SerializationTest, BCSMapToString)
{
    // Instantiate some ISerializable objects to use as values in the BCSMap
    auto boolValue1 = std::make_shared<Bool>(true);
    auto boolValue2 = std::make_shared<Bool>(false);

    // Create a BCSMap
    std::map<BString, std::shared_ptr<ISerializable>> mapValues = {
        {BString("key1"), boolValue1},
        {BString("key2"), boolValue2}};
    BCSMap map(mapValues);

    // Convert the map to a string
    std::string actual = map.ToString();

    // Expected output
    // The expected output depends on the implementation of BCSMap, BString, and Bool.
    // Modify this to match your expected output.
    std::string expected = "(key1, true)(key2, false)";

    // Check if the string matches the expected output
    EXPECT_EQ(expected, actual);
}

TEST(SerializationTest, BString)
{
    // Create a string to be used in the test
    std::string testStr = "Test string";

    // Create a BString with the test string
    BString bStr(testStr);

    // Use the ToString method to retrieve the string
    std::string actualStr = bStr.ToString();

    // Check if the returned string matches the input string
    EXPECT_EQ(testStr, actualStr);
}

TEST(BStringTest, GetHashCode)
{
    // Create a string to be used in the test
    std::string testStr = "Test string";

    // Create a BString with the test string
    BString bStr(testStr);

    // Use the GetHashCode method to retrieve the hash code
    size_t hashCode1 = bStr.GetHashCode();

    // Call GetHashCode again and ensure the hash code is the same
    size_t hashCode2 = bStr.GetHashCode();

    EXPECT_EQ(hashCode1, hashCode2);
}

TEST(BStringTest, Equals)
{
    // Create two BString objects with the same value
    BString bStr1("Test string");
    BString bStr2("Test string");

    // The two BString objects should be equal
    EXPECT_TRUE(bStr1.Equals(bStr2));

    // Create a third BString object with a different value
    BString bStr3("Different string");

    // The first and third BString objects should not be equal
    EXPECT_FALSE(bStr1.Equals(bStr3));
}

TEST(BStringTest, EqualOperator)
{
    // Create two BString objects with the same value
    BString bStr1("Test string");
    BString bStr2("Test string");

    // Use the == operator to compare the BString objects
    EXPECT_TRUE(bStr1 == bStr2);

    // Create another BString object with a different value
    BString bStr3("Another string");

    // Use the == operator to compare the first BString object with the third one
    EXPECT_FALSE(bStr1 == bStr3);
}

TEST(BStringTest, RemoveBOM)
{
    // Create a BString object
    BString bstring;

    // Create a vector of bytes with a BOM and some additional data
    std::vector<uint8_t> dataWithBOM = {0xEF, 0xBB, 0xBF, 'T', 'e', 's', 't'};

    // Use the RemoveBOM method to remove the BOM
    std::vector<uint8_t> resultWithBOM = bstring.RemoveBOM(dataWithBOM);

    // Create the expected result (the original data without the BOM)
    std::vector<uint8_t> expectedWithBOM = {'T', 'e', 's', 't'};

    // Check if the returned data matches the expected result
    EXPECT_EQ(expectedWithBOM, resultWithBOM);

    // Create a vector of bytes without a BOM
    std::vector<uint8_t> dataWithoutBOM = {'T', 'e', 's', 't'};

    // Use the RemoveBOM method to process the data
    std::vector<uint8_t> resultWithoutBOM = bstring.RemoveBOM(dataWithoutBOM);

    // The result should be the same as the input, since there was no BOM to remove
    EXPECT_EQ(dataWithoutBOM, resultWithoutBOM);
}

TEST(BoolTest, Variant)
{
    // Create a Bool object
    Bool b(true); // assuming Bool has a constructor that takes a bool

    // Use the Variant method and check if it returns TypeTag::BOOL
    EXPECT_EQ(b.Variant(), TypeTag::BOOL);
}

TEST(BoolTest, GetValue)
{
    // Create a Bool object with the value true
    Bool bTrue(true);

    // Use the GetValue method and check if it returns true
    EXPECT_TRUE(bTrue.GetValue());

    // Create a Bool object with the value false
    Bool bFalse(false);

    // Use the GetValue method and check if it returns false
    EXPECT_FALSE(bFalse.GetValue());
}

TEST(BoolTest, GetHashCode)
{
    // Create two Bool objects with the same value
    Bool bTrue1(true);
    Bool bTrue2(true);

    // The hashes of two identical objects should be the same
    EXPECT_EQ(bTrue1.GetHashCode(), bTrue2.GetHashCode());

    // Create a Bool object with a different value
    Bool bFalse(false);

    // The hash of a different object should (with high probability) be different
    EXPECT_NE(bTrue1.GetHashCode(), bFalse.GetHashCode());
}

TEST(DeserializationTest, Remaining)
{
    // Create a vector of bytes
    std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04, 0xFF};

    // Use the constructor to create a Deserialization object
    Deserialization deserializer(data);

    // Initially, the remaining data should be the size of the whole buffer
    EXPECT_EQ(data.size(), deserializer.Remaining());
}

TEST(DeserializationTest, FixedBytes)
{
    // Create a vector of bytes
    std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04, 0xFF};

    // Use the constructor to create a Deserialization object
    Deserialization deserializer(data);

    // Read some bytes
    std::size_t length = 3;
    std::vector<uint8_t> readData = deserializer.FixedBytes(length);

    // Check that the read data is correct
    std::vector<uint8_t> expectedData(data.begin(), data.begin() + length);
    EXPECT_EQ(expectedData, readData);

    // Check that the remaining data is correct
    EXPECT_EQ(data.size() - length, deserializer.Remaining());
}

TEST(DeserializationTest, DeserializeU16)
{
    // Create a vector of bytes representing uint16_t value of 500
    // Assuming the system uses little endian byte order
    std::vector<uint8_t> data = {0xF4, 0x01};

    // Use the constructor to create a Deserialization object
    Deserialization deserializer(data);

    // Deserialize the bytes to uint16_t
    uint16_t result = deserializer.DeserializeU16();

    // Check that the deserialized value is correct
    uint16_t expectedResult = 500;
    EXPECT_EQ(expectedResult, result);
}

TEST(DeserializationTest, DeserializeU256)
{
    // Create a Deserialization object with a known sequence of bytes
    // representing a 256-bit integer
    std::vector<uint8_t> known_data = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    Deserialization deserializer(known_data);

    // Call the DeserializeU256() method
    CryptoPP::Integer returned_value;
    ASSERT_NO_THROW(returned_value = deserializer.DeserializeU256());

    // Check the returned Integer
    CryptoPP::Integer known_value(1);
    EXPECT_EQ(returned_value, known_value);

    // Pass a short byte array and check that it throws a runtime_error
    std::vector<uint8_t> short_data = {0x01, 0x00, 0x00, 0x00};
    deserializer = Deserialization(short_data);
    ASSERT_THROW(deserializer.DeserializeU256(), std::runtime_error);
}

TEST(U32Test, Variant)
{
    // Create a U32 object
    uint32_t value = 12345;
    U32 u32(value);

    // Call the Variant method
    TypeTag variant = u32.Variant();

    // Assert that the variant is TypeTag::U8
    EXPECT_EQ(variant, TypeTag::U32);
}

TEST(U32Test, GetValue)
{
    // Create a U32 object with a known value
    uint32_t known_value = 12345;
    U32 u32(known_value);

    // Call the GetValue method
    uint32_t returned_value = u32.GetValue();

    // Assert that the returned value is the same as the known value
    EXPECT_EQ(returned_value, known_value);
}

TEST(U32Test, Equals)
{
    // Create two U32 objects with the same value
    uint32_t value1 = 12345;
    U32 u321(value1);

    uint32_t value2 = 12345;
    U32 u322(value2);

    // Assert that the two U32 objects are equal
    EXPECT_TRUE(u321.Equals(u322));

    // Create another U32 object with a different value
    uint32_t value3 = 67890;
    U32 u323(value3);

    // Assert that the first U32 object is not equal to the third one
    EXPECT_FALSE(u321.Equals(u323));
}

TEST(U32Test, ToString)
{
    // Create a U32 object with a known value
    uint32_t known_value = 12345;
    U32 u32(known_value);

    // Call the ToString method
    std::string returned_str = u32.ToString();

    // Assert that the returned string is the same as the string representation of the known value
    std::ostringstream oss;
    oss << static_cast<int>(known_value);
    std::string expected_str = oss.str();
    EXPECT_EQ(returned_str, expected_str);
}

TEST(U32Test, GetHashCode)
{
    // Create a U32 object with a known value
    uint32_t known_value = 12345;
    U32 u32(known_value);

    // Call the GetHashCode method
    size_t returned_hash = u32.GetHashCode();

    // Calculate the expected hash code using std::hash
    size_t expected_hash = std::hash<uint32_t>{}(known_value);

    // Assert that the returned hash code is the same as the expected hash code
    EXPECT_EQ(returned_hash, expected_hash);

    // Call GetHashCode again and ensure the hash code is the same
    size_t returned_hash_again = u32.GetHashCode();
    EXPECT_EQ(returned_hash, returned_hash_again);
}

TEST(U32Test, Deserialize)
{
    // Create a known uint32_t value and its corresponding byte representation
    uint32_t known_value = 12345;
    std::vector<uint8_t> byte_representation(sizeof(uint32_t));
    std::memcpy(byte_representation.data(), &known_value, sizeof(uint32_t));

    // Call the Deserialize method
    uint32_t deserialized_value;
    ASSERT_NO_THROW(deserialized_value = U32::Deserialize(byte_representation));

    // Assert that the deserialized value is the same as the known value.
    // This assumes that the system where the test runs uses little endian byte order.
    // If that's not the case, you need to convert deserialized_value to the correct byte order.
    EXPECT_EQ(deserialized_value, known_value);

    // Test that calling Deserialize with a vector of incorrect size throws an exception
    std::vector<uint8_t> small_data(sizeof(uint32_t) - 1);
    EXPECT_THROW(U32::Deserialize(small_data), std::runtime_error);
}

TEST(U8Test, Constructor)
{
    // Create a U8 object with a known value
    uint8_t known_value = 123;
    U8 u8(known_value);

    // Assert that the value of the U8 object is the same as the known value
    EXPECT_EQ(u8.GetValue(), known_value);
}

TEST(U8Test, Serialize)
{
    // Create a U8 object with a known value
    uint8_t known_value = 123;
    U8 u8(known_value);

    // Create a serializer
    Serialization serializer;

    // Call the Serialize method and verify it does not throw exception
    ASSERT_NO_THROW(u8.Serialize(serializer));
}

TEST(U8Test, Variant)
{
    // Create a U8 object
    U8 u8(123);

    // Call the Variant method
    TypeTag variant = u8.Variant();

    // Assert that the returned TypeTag is U8
    EXPECT_EQ(variant, TypeTag::U8);
}

TEST(U8Test, Equals)
{
    // Create two U8 objects with the same value
    U8 u8_1(123);
    U8 u8_2(123);

    // Assert that Equals returns true when the two U8 objects have the same value
    EXPECT_TRUE(u8_1.Equals(u8_2));

    // Create another U8 object with a different value
    U8 u8_3(200);

    // Assert that Equals returns false when the two U8 objects have different values
    EXPECT_FALSE(u8_1.Equals(u8_3));
}

TEST(U8Test, ToString)
{
    // Create a U8 object with a known value
    uint8_t known_value = 123;
    U8 u8(known_value);

    // Call the ToString method
    std::string str = u8.ToString();

    // Assert that the returned string is the string representation of the known value
    EXPECT_EQ(str, "123");
}

TEST(U8Test, GetHashCode)
{
    // Create two U8 objects with the same value
    uint8_t known_value = 123;
    U8 u8_1(known_value);
    U8 u8_2(known_value);

    // Assert that GetHashCode returns the same hash code for two equal U8 objects
    EXPECT_EQ(u8_1.GetHashCode(), u8_2.GetHashCode());

    // Create another U8 object with a different value
    U8 u8_3(200);

    // Assert that GetHashCode returns different hash codes for two different U8 objects
    // Note: This assertion could theoretically fail if a hash collision occurred, but this is highly unlikely with std::hash and small integers
    EXPECT_NE(u8_1.GetHashCode(), u8_3.GetHashCode());
}

TEST(U8Test, Deserialize)
{
    // Create a vector of bytes
    std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04, 0xFF};

    // Use the constructor to create a Deserialization object
    Deserialization deserializer(data);

    // Call the Deserialize method and ensure it does not throw exception
    ASSERT_NO_THROW(U8::Deserialize(deserializer));
}

TEST(U64Test, Serialize)
{
    // Create a U64 object with a known value
    uint64_t known_value = 1234567890123456789;
    U64 u64(known_value);

    // Create a serializer
    Serialization serializer;

    // Call the Serialize method and verify it does not throw exception
    ASSERT_NO_THROW(u64.Serialize(serializer));
}

TEST(U64Test, Variant)
{
    // Create a U64 object
    uint64_t known_value = 1234567890123456789;
    U64 u64(known_value);

    // Call the Variant method
    TypeTag typeTag = u64.Variant();

    // Assert that the returned TypeTag is TypeTag::U8
    EXPECT_EQ(typeTag, TypeTag::U64);
}

TEST(U64Test, GetValue)
{
    // Create a U64 object with a known value
    uint64_t known_value = 1234567890123456789;
    U64 u64(known_value);

    // Call the GetValue method
    uint64_t retrieved_value = u64.GetValue();

    // Assert that the retrieved value matches the known value
    EXPECT_EQ(retrieved_value, known_value);
}

TEST(U64Test, Equals)
{
    // Create two U64 objects with the same value
    uint64_t known_value = 1234567890123456789;
    U64 u64_1(known_value);
    U64 u64_2(known_value);

    // Assert that Equals returns true when comparing two objects with the same value
    EXPECT_TRUE(u64_1.Equals(u64_2));

    // Create a U64 object with a different value
    uint64_t different_value = 43210987654321;
    U64 u64_3(different_value);

    // Assert that Equals returns false when comparing two objects with different values
    EXPECT_FALSE(u64_1.Equals(u64_3));
}

TEST(U64Test, ToString)
{
    // Create a U64 object with a known value
    uint64_t known_value = 2112454933;
    U64 u64(known_value);

    // Create a string representation of the known value
    std::ostringstream oss;
    oss << known_value;
    std::string expected_string = oss.str();

    // Call the ToString method
    std::string returned_string = u64.ToString();

    // Assert that the returned string matches the expected string
    EXPECT_EQ(returned_string, expected_string);
}

TEST(U64Test, GetHashCode)
{
    // Create a U64 object with a known value
    uint64_t known_value = 2112454933;
    U64 u64(known_value);

    // Compute expected hash code
    size_t expected_hash_code = std::hash<uint64_t>{}(known_value);

    // Call the GetHashCode method
    size_t returned_hash_code = u64.GetHashCode();

    // Assert that the returned hash code matches the expected hash code
    EXPECT_EQ(returned_hash_code, expected_hash_code);
}

TEST(U64Test, Deserialize)
{
    // Create a known uint64_t value
    uint64_t known_value = 1234567890123456789;
    uint64_t little_endian_value;

    if constexpr (std::endian::native == std::endian::little)
    {
        little_endian_value = known_value;
    }
    else
    {
        // Convert the known_value to little-endian byte order
        little_endian_value = (static_cast<uint64_t>(known_value) << 56) |
                              ((static_cast<uint64_t>(known_value) & 0x000000000000FF00) << 40) |
                              ((static_cast<uint64_t>(known_value) & 0x0000000000FF0000) << 24) |
                              ((static_cast<uint64_t>(known_value) & 0x00000000FF000000) << 8) |
                              ((static_cast<uint64_t>(known_value) & 0x000000FF00000000) >> 8) |
                              ((static_cast<uint64_t>(known_value) & 0x0000FF0000000000) >> 24) |
                              ((static_cast<uint64_t>(known_value) & 0x00FF000000000000) >> 40) |
                              (static_cast<uint64_t>(known_value) >> 56);
        ;
    }

    // Prepare the byte data
    std::vector<uint8_t> data(sizeof(uint64_t));
    std::memcpy(data.data(), &little_endian_value, sizeof(uint64_t));

    // Call the Deserialize method and ensure it does not throw exception
    uint64_t deserialized_value;
    ASSERT_NO_THROW(deserialized_value = U64::Deserialize(data));

    // Check that the deserialized value is equal to the known value
    ASSERT_EQ(deserialized_value, known_value);

    // Test with not enough bytes
    data.resize(sizeof(uint64_t) - 1);

    // Call the Deserialize method and ensure it throws an exception
    ASSERT_THROW(U64::Deserialize(data), std::runtime_error);
}

TEST(U16Test, Serialize)
{
    // Create a U16 object with a known value
    uint16_t known_value = 12345;
    U16 u16(known_value);

    // Create a mock Serialization object
    Serialization serializer;

    // Call the Serialize method and verify it does not throw an exception
    ASSERT_NO_THROW(u16.Serialize(serializer));
}

TEST(U16Test, Variant)
{
    // Create a U16 object
    U16 u16(12345);

    // Call the Variant method
    TypeTag variant = u16.Variant();

    // Check that the returned variant is TypeTag::U8
    EXPECT_EQ(variant, TypeTag::U16);
}

TEST(U16Test, GetValue)
{
    // Create a U16 object with a known value
    uint16_t known_value = 12345;
    U16 u16(known_value);

    // Call the GetValue method
    uint16_t value = u16.GetValue();

    // Check that the returned value is equal to the known value
    EXPECT_EQ(value, known_value);
}

TEST(U16Test, Equals)
{
    // Create two U16 objects with the same value
    uint16_t known_value = 12345;
    U16 u16_1(known_value);
    U16 u16_2(known_value);

    // Call the Equals method and check that the two objects are equal
    EXPECT_TRUE(u16_1.Equals(u16_2));

    // Create a U16 object with a different value
    U16 u16_3(known_value + 1);

    // Call the Equals method and check that the two objects are not equal
    EXPECT_FALSE(u16_1.Equals(u16_3));
}

TEST(U16Test, ToString)
{
    // Create a U16 object with a known value
    uint16_t known_value = 12345;
    U16 u16(known_value);

    // Call the ToString method
    std::string str = u16.ToString();

    // Check that the returned string is equal to the string representation of the known value
    EXPECT_EQ(str, std::to_string(known_value));
}

TEST(U16Test, GetHashCode)
{
    // Create a U16 object with a known value
    uint16_t known_value = 12345;
    U16 u16(known_value);

    // Call the GetHashCode method
    size_t hash_code = u16.GetHashCode();

    // Calculate the expected hash code
    size_t expected_hash_code = std::hash<uint16_t>{}(known_value);

    // Check that the returned hash code is equal to the expected hash code
    EXPECT_EQ(hash_code, expected_hash_code);
}

TEST(U16Test, Deserialize)
{
    // Create a known uint64_t value
    uint16_t known_value = 1234567890123456789;
    uint16_t little_endian_value;

    if constexpr (std::endian::native == std::endian::little)
    {
        little_endian_value = known_value;
    }
    else
    {
        // Convert the known_value to little-endian byte order
        little_endian_value = (known_value >> 8) | (known_value << 8);
    }

    // Prepare the byte data
    std::vector<uint8_t> data(sizeof(uint16_t));
    std::memcpy(data.data(), &little_endian_value, sizeof(uint16_t));

    // Call the Deserialize method and ensure it does not throw exception
    uint16_t deserialized_value;
    ASSERT_NO_THROW(deserialized_value = U16::Deserialize(data));

    // Check that the deserialized value is equal to the known value
    ASSERT_EQ(deserialized_value, known_value);

    // Test with not enough bytes
    data.resize(sizeof(uint16_t) - 1);

    // Call the Deserialize method and ensure it throws an exception
    ASSERT_THROW(U16::Deserialize(data), std::runtime_error);
}

TEST(U128Test, Serialize)
{
    // Create a U128 object with a known value
    CryptoPP::Integer known_value("1234567890123456789012345678901234567890");
    U128 u128(known_value);

    // Create a mock Serialization object
    Serialization serializer;

    // Call the Serialize method and verify it does not throw an exception
    ASSERT_NO_THROW(u128.Serialize(serializer));
}

TEST(U128Test, ToString)
{
    // Create a U128 object with a known value
    CryptoPP::Integer known_value("1234567890123456789012345678901234567890");
    U128 u128(known_value);

    // Call the ToString method
    std::string str = u128.ToString();

    // Convert the known value to a string
    std::ostringstream oss;
    oss << known_value;

    // Check that the returned string is equal to the string representation of the known value
    EXPECT_EQ(str, oss.str());
}

TEST(U128Test, Equals)
{
    // Create two U128 objects with the same value
    CryptoPP::Integer known_value("1234567890123456789012345678901234567890");
    U128 u128_1(known_value);
    U128 u128_2(known_value);

    // Call the Equals method and check that the two objects are equal
    EXPECT_TRUE(u128_1.Equals(u128_2));

    // Create a U128 object with a different value
    CryptoPP::Integer different_value("9876543210987654321098765432109876543210");
    U128 u128_3(different_value);

    // Call the Equals method and check that the two objects are not equal
    EXPECT_FALSE(u128_1.Equals(u128_3));
}

TEST(U128Test, Variant)
{
    // Create a U128 object
    CryptoPP::Integer known_value("1234567890123456789012345678901234567890");
    U128 u128(known_value);

    // Call the Variant method
    TypeTag variant = u128.Variant();

    // Check that the returned variant is TypeTag::U128
    EXPECT_EQ(variant, TypeTag::U128);
}

TEST(U128Test, GetValue)
{
    // Create a U128 object with a known value
    CryptoPP::Integer known_value("1234567890123456789012345678901234567890");
    U128 u128(known_value);

    // Call the GetValue method
    CryptoPP::Integer returned_value = u128.GetValue();

    // Check that the returned value is equal to the known value
    EXPECT_EQ(returned_value, known_value);
}

TEST(U128Test, GetHashCode)
{
    // Create a U128 object with a known value
    CryptoPP::Integer known_value("1234567890123456789012345678901234567890");
    U128 u128(known_value);

    // Call the GetHashCode method and check it does not throw an exception
    size_t hash_code;
    ASSERT_NO_THROW(hash_code = u128.GetHashCode());

    // Check that the returned hash code is not 0
    // (Assuming that the hash of a non-empty string will never be 0)
    EXPECT_NE(hash_code, 0);
}

TEST(U128Test, Deserialize)
{
    // Create a std::vector<uint8_t> with known contents
    std::vector<uint8_t> data = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    // Call the Deserialize method
    CryptoPP::Integer returned_value = U128::Deserialize(data);

    // Check that the returned value is equal to the known value
    CryptoPP::Integer known_value(1);
    EXPECT_EQ(returned_value, known_value);

    // Try to deserialize a vector that is too short, and check that an exception is thrown
    std::vector<uint8_t> short_data = {1, 0, 0};
    EXPECT_THROW(U128::Deserialize(short_data), std::runtime_error);
}

TEST(U256Test, Serialize)
{
    // Create a mock Serialization object
    Serialization serializer;

    // Create a U256 object with a known value
    CryptoPP::Integer known_value("1234567890123456789012345678901234567890");
    U256 u256(known_value);

    // Call the Serialize method
    u256.Serialize(serializer);
}

TEST(U256Test, ToString)
{
    // Create a U256 object with a known value
    CryptoPP::Integer known_value("2345678901234567890");
    U256 u256(known_value);

    // Call the ToString method
    std::string str = u256.ToString();

    // Convert the known value to a string
    std::ostringstream oss;
    oss << known_value;

    // Check that the returned string is equal to the string representation of the known value
    EXPECT_EQ(str, oss.str());
}

TEST(U256Test, Equals)
{
    // Create a U256 object with a known value
    CryptoPP::Integer known_value("1234567890123456789012345678901234567890");
    U256 u256(known_value);

    // Create another U256 object with the same value
    U256 u256_same(known_value);

    // Call the Equals method and check that it returns true
    EXPECT_TRUE(u256.Equals(u256_same));

    // Create a U256 object with a different value
    CryptoPP::Integer different_value("9876543210987654321098765432109876543210");
    U256 u256_different(different_value);

    // Call the Equals method and check that it returns false
    EXPECT_FALSE(u256.Equals(u256_different));
}

TEST(U256Test, Variant)
{
    // Create a U256 object
    CryptoPP::Integer known_value("1234567890123456789012345678901234567890");
    U256 u256(known_value);

    // Call the Variant method
    TypeTag tag = u256.Variant();

    // Check that the returned tag is U256
    EXPECT_EQ(tag, TypeTag::U256);
}

TEST(U256Test, GetValue)
{
    // Create a U256 object with a known value
    CryptoPP::Integer known_value("1234567890123456789012345678901234567890");
    U256 u256(known_value);

    // Call the GetValue method
    CryptoPP::Integer returned_value = u256.GetValue();

    // Check that the returned value is equal to the known value
    EXPECT_EQ(returned_value, known_value);
}

TEST(U256Test, GetHashCode)
{
    // Create a U256 object with a known value
    CryptoPP::Integer known_value("1234567890123456789012345678901234567890");
    U256 u256(known_value);

    // Call the GetHashCode method and check it does not throw an exception
    size_t hash_code;
    ASSERT_NO_THROW(hash_code = u256.GetHashCode());

    // Check that the returned hash code is not 0
    // (Assuming that the hash of a non-empty string will never be 0)
    EXPECT_NE(hash_code, 0);
}

TEST(U256Test, Deserialize)
{
    // Create a byte array with a known value
    std::vector<uint8_t> known_data = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    // Deserialize the byte array
    CryptoPP::Integer returned_value;
    ASSERT_NO_THROW(returned_value = U256::Deserialize(known_data));

    // Check that the returned value is equal to the known value
    CryptoPP::Integer known_value(1);
    EXPECT_EQ(returned_value, known_value);

    // Pass a short byte array and check that it throws a runtime_error
    std::vector<uint8_t> short_data = {0x01, 0x00, 0x00, 0x00};
    ASSERT_THROW(U256::Deserialize(short_data), std::runtime_error);
}

TEST(BytesTest, GetValue)
{
    // Create a Bytes object with a known vector of bytes
    std::vector<uint8_t> known_values = {1, 2, 3, 4, 5};
    Bytes bytes(known_values);

    // Call the getValue method
    std::vector<uint8_t> returned_values = bytes.getValue();

    // Check the vectors match
    EXPECT_EQ(returned_values, known_values);
}

TEST(BytesTest, Equals)
{
    // Create two Bytes objects with the same vector of bytes
    std::vector<uint8_t> known_values1 = {1, 2, 3, 4, 5};
    Bytes bytes1(known_values1);
    std::vector<uint8_t> known_values2 = {1, 2, 3, 4, 5};
    Bytes bytes2(known_values2);

    // Call the Equals method and check that it returns true
    ASSERT_TRUE(bytes1.Equals(bytes2));

    // Create two Bytes objects with different vectors of bytes
    std::vector<uint8_t> known_values3 = {1, 2, 3, 4, 5};
    Bytes bytes3(known_values3);
    std::vector<uint8_t> known_values4 = {5, 4, 3, 2, 1};
    Bytes bytes4(known_values4);

    // Call the Equals method and check that it returns false
    ASSERT_FALSE(bytes3.Equals(bytes4));
}

TEST(BytesTest, ToString)
{
    // Create a Bytes object with a known vector of bytes
    std::vector<uint8_t> known_values = {1, 2, 3, 4, 5};
    Bytes bytes(known_values);

    // Call the ToString method
    std::string returned_string = bytes.ToString();

    // Check the returned string
    std::string expected_string = "12345"; // This depends on how the ToString function is implemented
    EXPECT_EQ(returned_string, expected_string);
}

TEST(BytesTest, GetHashCode)
{
    // Create a Bytes object with a known vector of bytes
    std::vector<uint8_t> known_values = {1, 2, 3, 4, 5};
    Bytes bytes(known_values);

    // Call the GetHashCode method
    size_t returned_hash = bytes.GetHashCode();

    // Check the returned hash code
    // Calculate the expected hash code
    std::string str = "12345"; // This is the string representation of the known_values
    size_t expected_hash = std::hash<std::string>{}(str);
    EXPECT_EQ(returned_hash, expected_hash);
}

TEST(BytesTest, Serialize)
{
    // Create a Bytes object with a known sequence of bytes
    std::vector<uint8_t> known_values = {0x01, 0x02, 0x03, 0x04, 0x05};
    Bytes bytes_obj(known_values);

    // Create a Serialization object
    Serialization serializer;

    // Call the Serialize method
    ASSERT_NO_THROW(bytes_obj.Serialize(serializer));
}

TEST(SequenceTest, Length)
{
    // Create a Sequence object with three values
    std::shared_ptr<ISerializable> value1; // Assume these are valid pointers to ISerializable objects
    std::shared_ptr<ISerializable> value2;
    std::shared_ptr<ISerializable> value3;
    std::vector<std::shared_ptr<ISerializable>> values = {value1, value2, value3};

    Sequence sequence(values);

    // The length of the sequence should be 3
    EXPECT_EQ(sequence.Length(), 3);

    // Create another Sequence object with two values
    std::shared_ptr<ISerializable> value4; // Assume these are valid pointers to ISerializable objects
    std::shared_ptr<ISerializable> value5;
    std::vector<std::shared_ptr<ISerializable>> otherValues = {value4, value5};

    Sequence otherSequence(otherValues);

    // The length of the other sequence should be 2
    EXPECT_EQ(otherSequence.Length(), 2);
}

TEST(SequenceTest, GetValue)
{
    // Create a Sequence object with three values
    std::shared_ptr<ISerializable> value1; // Assume these are valid pointers to ISerializable objects
    std::shared_ptr<ISerializable> value2;
    std::shared_ptr<ISerializable> value3;
    std::vector<std::shared_ptr<ISerializable>> values = {value1, value2, value3};

    Sequence sequence(values);

    // The values returned by GetValue should be the same as the values we used to create the sequence
    std::vector<std::shared_ptr<ISerializable>> returnedValues = sequence.GetValue();
    EXPECT_EQ(returnedValues.size(), values.size());
    for (size_t i = 0; i < values.size(); i++)
    {
        EXPECT_EQ(returnedValues[i], values[i]);
    }
}

TEST(SequenceTest, ToString)
{
    // Instantiate some ISerializable objects to use as values in the Sequence
    Bool boolValue1(true);
    Bool boolValue2(false);
    BString stringValue1("key1");
    BString stringValue2("key2");

    // Create a Sequence
    std::vector<std::shared_ptr<ISerializable>> sequenceValues = {
        std::make_shared<Bool>(boolValue1),
        std::make_shared<BString>(stringValue1),
        std::make_shared<Bool>(boolValue2),
        std::make_shared<BString>(stringValue2)};

    Sequence sequence(sequenceValues);

    // Convert the Sequence to a string
    std::string actual = sequence.ToString();

    // Expected output
    std::string expected = "truekey1falsekey2";

    // Check if the string matches the expected output
    EXPECT_EQ(expected, actual);
}

TEST(SequenceTest, GetHashCode)
{
    // Create a Sequence object with three values
    std::shared_ptr<ISerializable> value1; // Assume these are valid pointers to ISerializable objects
    std::shared_ptr<ISerializable> value2;
    std::shared_ptr<ISerializable> value3;
    std::vector<std::shared_ptr<ISerializable>> values = {value1, value2, value3};
    // Create a BString with the test string
    Sequence sequence(values);

    // Use the GetHashCode method to retrieve the hash code
    size_t hashCode1 = sequence.GetHashCode();

    // Call GetHashCode again and ensure the hash code is the same
    size_t hashCode2 = sequence.GetHashCode();

    EXPECT_EQ(hashCode1, hashCode2);
}

TEST(SequenceTest, Deserialize)
{
    // Create a known Sequence value
    std::vector<uint8_t> bytes1 = {1};
    std::vector<uint8_t> bytes2 = {2};
    std::vector<uint8_t> bytes3 = {3};
    std::shared_ptr<ISerializable> value1 = std::make_shared<Bytes>(bytes1);
    std::shared_ptr<ISerializable> value2 = std::make_shared<Bytes>(bytes2);
    std::shared_ptr<ISerializable> value3 = std::make_shared<Bytes>(bytes3);
    std::vector<std::shared_ptr<ISerializable>> known_values = {value1, value2, value3};
    std::shared_ptr<ISerializable> known_sequence = std::make_shared<Sequence>(known_values);
    std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04, 0xFF};

    // Create a Deserialization object with the serialized data
    Deserialization deserializer(data);

    // Call the Deserialize method and ensure it does not throw exception
    std::shared_ptr<ISerializable> deserialized_sequence;
    ASSERT_NO_THROW(deserialized_sequence = Sequence::Deserialize(deserializer));

    // Test with not enough bytes
    data.resize(data.size() - 1);
    Deserialization bad_deserializer(data);
}

TEST(SerializationTest, SerializeU8)
{
    // Create a mock Serialization object
    Serialization serializer;

    // Create a uint8_t with known value
    uint8_t known_value = 123; // this can be any value you want to test

    // Call the Serialize method
    serializer.Serialize(known_value);

    // Get the serialized bytes
    std::vector<uint8_t> serialized_bytes = serializer.GetBytes();

    // Check that the serialized bytes match the known value
    // Since uint8_t is just a single byte, the serialized bytes should be a vector of length 1
    ASSERT_EQ(serialized_bytes.size(), 1);
    ASSERT_EQ(serialized_bytes[0], known_value);
}

TEST(SerializationTest, SerializeU64)
{
    // Create a mock Serialization object
    Serialization serializer;

    // Create a uint64_t with a known value
    uint64_t known_value = 123456789012345678; // this can be any value you want to test

    // Call the Serialize method
    serializer.Serialize(known_value);

    // Get the serialized bytes
    std::vector<uint8_t> serialized_bytes = serializer.GetBytes();

    // Check that the serialized bytes match the known value
    // Since uint64_t is eight bytes, the serialized bytes should be a vector of length 8
    // Also, we need to account for endianness. Assuming you're using little-endian order:
    ASSERT_EQ(serialized_bytes.size(), 8);
    for (int i = 0; i < 8; i++)
    {
        ASSERT_EQ(serialized_bytes[i], static_cast<uint8_t>((known_value >> (8 * i)) & 0xFF));
    }
}

TEST(SerializationTest, SerializeU128)
{
    // Create a mock Serialization object
    Serialization serializer;

    // Create a CryptoPP::Integer with a known value
    CryptoPP::Integer known_value("1234567890123456789012345678901234567890");

    // Call the Serialize method
    serializer.Serialize(known_value);

    // Get the serialized bytes
    std::vector<uint8_t> serialized_bytes = serializer.GetBytes();

    // Check that the serialized bytes match the known value
    // Since we're dealing with a 128-bit integer, the serialized bytes should be a vector of length 16
    // Also, we need to account for endianness. Assuming you're using little-endian order:
    ASSERT_EQ(serialized_bytes.size(), 16);
    for (int i = 0; i < 16; i++)
    {
        // Convert the CryptoPP::Integer to bytes
        uint8_t byte = static_cast<uint8_t>(known_value.GetByte(i));
        ASSERT_EQ(serialized_bytes[i], byte);
    }
}

TEST(SerializationTest, SerializeVectorU8)
{
    // Create a mock Serialization object
    Serialization serializer;

    // Create a vector of bytes with known values
    std::vector<uint8_t> known_values = {1, 2, 3, 4, 5};

    // Call the Serialize method
    serializer.Serialize(known_values);

    // Get the serialized bytes
    std::vector<uint8_t> serialized_bytes = serializer.GetBytes();

    // Create the expected serialized bytes, including the size byte
    std::vector<uint8_t> expected_bytes;
    expected_bytes.push_back(static_cast<uint8_t>(known_values.size()));
    expected_bytes.insert(expected_bytes.end(), known_values.begin(), known_values.end());

    // Check that the serialized bytes match the expected bytes
    ASSERT_EQ(serialized_bytes, expected_bytes);
}

TEST(SerializationTest, SerializeU16)
{
    // Create a mock Serialization object
    Serialization serializer;

    // Create a uint16_t with a known value
    uint16_t known_value = 12345; // this can be any value you want to test

    // Call the Serialize method
    serializer.Serialize(known_value);

    // Get the serialized bytes
    std::vector<uint8_t> serialized_bytes = serializer.GetBytes();

    // Check that the serialized bytes match the known value
    ASSERT_EQ(serialized_bytes.size(), 2);

    // Test for little-endian order
    for (int i = 0; i < 2; i++)
    {
        uint8_t expected_byte = static_cast<uint8_t>((known_value >> (8 * i)) & 0xFF);
        ASSERT_EQ(serialized_bytes[i], expected_byte)
            << "Failed at index " << i << ". "
            << "Expected: " << static_cast<int>(expected_byte) << ", "
            << "but was: " << static_cast<int>(serialized_bytes[i]) << ".";
    }
}

TEST(TagSequenceTest, GetValueTest)
{
    AccountAddress accountAddress = AccountAddress::FromHex("0x1");
    std::string module = "aptos_coin";
    std::string name = "AptosCoin";
    std::vector<std::shared_ptr<ISerializableTag>> tags;

    StructTag structTag(accountAddress, module, name, tags);
    std::vector<std::shared_ptr<ISerializableTag>> typeTags = {std::make_shared<StructTag>(structTag)};

    TagSequence typeTagsSequence(typeTags);

    // Get the value of the sequence
    auto actualTags = typeTagsSequence.GetValue();

    // Check that the returned tags match the input tags
    EXPECT_EQ(typeTags, actualTags);
}

TEST(TagSequenceTest, ToStringTest)
{
    AccountAddress accountAddress = AccountAddress::FromHex("0x1");
    std::string module = "aptos_coin";
    std::string name = "AptosCoin";
    std::vector<std::shared_ptr<ISerializableTag>> tags;

    StructTag structTag(accountAddress, module, name, tags);
    std::vector<std::shared_ptr<ISerializableTag>> typeTags = {std::make_shared<StructTag>(structTag)};

    TagSequence typeTagsSequence(typeTags);

    // Call the ToString method
    std::string actual = typeTagsSequence.ToString();

    // Compute the expected string
    std::ostringstream oss;
    for (const auto &tag : typeTags)
    {
        oss << tag->ToString();
    }
    std::string expected = oss.str();

    // Check that the returned string matches the expected string
    EXPECT_EQ(expected, actual);
}

TEST(StructTagTest, ToStringTest)
{
    AccountAddress accountAddress = AccountAddress::FromHex("0x1");
    std::string module = "aptos_coin";
    std::string name = "AptosCoin";
    std::vector<std::shared_ptr<ISerializableTag>> typeArgs; // Fill this with actual ISerializableTag instances

    StructTag structTag(accountAddress, module, name, typeArgs);

    // Call the ToString method
    std::string actual = structTag.ToString();

    // Compute the expected string
    std::string expected = accountAddress.ToString() + "::" + module + "::" + name;
    if (!typeArgs.empty())
    {
        expected += "<" + typeArgs[0]->ToString();
        for (size_t i = 1; i < typeArgs.size(); ++i)
        {
            expected += ", " + typeArgs[i]->ToString();
        }
        expected += ">";
    }

    // Check that the returned string matches the expected string
    EXPECT_EQ(expected, actual);
}

TEST(StructTagTest, FromStrTest)
{
    // Define the typeTag string
    std::string typeTag = "0x1::aptos_coin::AptosCoin";

    // Call the FromStr method
    StructTag actual = StructTag::FromStr(typeTag);

    // Define the expected StructTag
    AccountAddress expectedAddress = AccountAddress::FromHex("0x1");
    std::string expectedModule = "aptos_coin";
    std::string expectedName = "AptosCoin";
    std::vector<std::shared_ptr<ISerializableTag>> expectedTypeArgs; // No type arguments in this case
    StructTag expected(expectedAddress, expectedModule, expectedName, expectedTypeArgs);

    // Check that the returned StructTag matches the expected StructTag
    EXPECT_EQ(expected, actual);
}

TEST(ISerializableTag, DeserializeTagBool)
{
    bool actual = true;
    Serialization s;
    s.SerializeBool(actual);
    std::vector<uint8_t> res;
    std::vector<uint8_t> data = s.GetBytes();
    res.push_back((uint8_t)TypeTag::BOOL);
    res.insert(res.end(), data.begin(), data.end());

    Deserialization d(res);
    std::shared_ptr<ISerializableTag> result = ISerializableTag::DeserializeTag(d);
    ASSERT_EQ(std::dynamic_pointer_cast<Bool>(result)->Variant(), TypeTag::BOOL);
    ASSERT_EQ(std::dynamic_pointer_cast<Bool>(result)->GetValue(), true);
}

TEST(ISerializableTag, DeserializeTagU8)
{
    Serialization s;
    s.SerializeU8(1);
    std::vector<uint8_t> res;
    std::vector<uint8_t> data = s.GetBytes();
    res.push_back((uint8_t)TypeTag::U8);
    res.insert(res.end(), data.begin(), data.end());

    Deserialization d(res);
    std::shared_ptr<ISerializableTag> result = ISerializableTag::DeserializeTag(d);
    ASSERT_EQ(std::dynamic_pointer_cast<U8>(result)->Variant(), TypeTag::U8);
    ASSERT_EQ(std::dynamic_pointer_cast<U8>(result)->GetValue(), 1);
    ASSERT_EQ(result->ISerializableTag::ToString(), "ISerializableTag (U8)");
}

TEST(ISerializableTag, DeserializeTagU16)
{
    std::vector<uint8_t> res;
    std::vector<uint8_t> data = {0xF4, 0x01};
    res.push_back((uint8_t)TypeTag::U16);
    res.insert(res.end(), data.begin(), data.end());

    Deserialization d(res);
    std::shared_ptr<ISerializableTag> result = ISerializableTag::DeserializeTag(d);
    ASSERT_EQ(std::dynamic_pointer_cast<U16>(result)->Variant(), TypeTag::U16);
    ASSERT_EQ(std::dynamic_pointer_cast<U16>(result)->GetValue(), 500);
    ASSERT_EQ(result->ISerializableTag::ToString(), "ISerializableTag (U16)");
}

TEST(ISerializableTag, DeserializeTagU32)
{
    Serialization s;
    s.SerializeU32(1);
    std::vector<uint8_t> res;
    std::vector<uint8_t> data = s.GetBytes();
    res.push_back((uint8_t)TypeTag::U32);
    res.insert(res.end(), data.begin(), data.end());

    Deserialization d(res);
    std::shared_ptr<ISerializableTag> result = ISerializableTag::DeserializeTag(d);
    ASSERT_EQ(std::dynamic_pointer_cast<U32>(result)->Variant(), TypeTag::U32);
    ASSERT_EQ(std::dynamic_pointer_cast<U32>(result)->GetValue(), 1);
    ASSERT_EQ(result->ISerializableTag::ToString(), "ISerializableTag (U32)");
}

TEST(ISerializableTag, DeserializeTagU64)
{
    Serialization s;
    s.SerializeU64(1);
    std::vector<uint8_t> res;
    std::vector<uint8_t> data = s.GetBytes();
    res.push_back((uint8_t)TypeTag::U64);
    res.insert(res.end(), data.begin(), data.end());

    Deserialization d(res);
    std::shared_ptr<ISerializableTag> result = ISerializableTag::DeserializeTag(d);
    ASSERT_EQ(std::dynamic_pointer_cast<U64>(result)->Variant(), TypeTag::U64);
    ASSERT_EQ(std::dynamic_pointer_cast<U64>(result)->GetValue(), 1);
    ASSERT_EQ(result->ISerializableTag::ToString(), "ISerializableTag (U64)");
}

TEST(ISerializableTag, DeserializeTagU128)
{
    Serialization s;
    s.SerializeU128(1);
    std::vector<uint8_t> res;
    std::vector<uint8_t> data = s.GetBytes();
    res.push_back((uint8_t)TypeTag::U128);
    res.insert(res.end(), data.begin(), data.end());

    Deserialization d(res);
    std::shared_ptr<ISerializableTag> result = ISerializableTag::DeserializeTag(d);
    ASSERT_EQ(std::dynamic_pointer_cast<U128>(result)->Variant(), TypeTag::U128);
    ASSERT_EQ(std::dynamic_pointer_cast<U128>(result)->GetValue(), 1);
    ASSERT_EQ(result->ISerializableTag::ToString(), "ISerializableTag (U128)");
}

TEST(ISerializableTag, DeserializeTagU256)
{
    Serialization s;
    s.SerializeU256(1);
    std::vector<uint8_t> res;
    std::vector<uint8_t> data = s.GetBytes();
    res.push_back((uint8_t)TypeTag::U256);
    res.insert(res.end(), data.begin(), data.end());

    Deserialization d(res);
    std::shared_ptr<ISerializableTag> result = ISerializableTag::DeserializeTag(d);
    ASSERT_EQ(std::dynamic_pointer_cast<U256>(result)->Variant(), TypeTag::U256);
    ASSERT_EQ(std::dynamic_pointer_cast<U256>(result)->GetValue(), 1);
    ASSERT_EQ(result->ISerializableTag::ToString(), "ISerializableTag (U256)");
}

TEST(ISerializableTag, DeserializeTagAccountAddress)
{
    CryptoPP::SecByteBlock publicKeyBytes = Aptos::Utils::ByteVectorToSecBlock({88, 110, 60, 141, 68, 125, 118, 121,
                                                                                34, 46, 19, 144, 51, 227, 130, 2,
                                                                                53, 227, 61, 165, 9, 30, 155, 11,
                                                                                184, 241, 161, 18, 207, 12, 143, 245});
    PublicKey publicKey(publicKeyBytes);
    AccountAddress accountAddress = AccountAddress::FromKey(publicKey.KeyBytes());
    Serialization s;
    accountAddress.Serialize(s);
    std::vector<uint8_t> res;
    std::vector<uint8_t> data = s.GetBytes();
    res.push_back((uint8_t)TypeTag::ACCOUNT_ADDRESS);
    res.insert(res.end(), data.begin(), data.end());

    Deserialization d(res);
    std::shared_ptr<ISerializableTag> result = ISerializableTag::DeserializeTag(d);
    ASSERT_EQ(std::dynamic_pointer_cast<AccountAddress>(result)->Variant(), TypeTag::ACCOUNT_ADDRESS);
    ASSERT_EQ(result->ISerializableTag::ToString(), "ISerializableTag (ACCOUNT_ADDRESS)");
}