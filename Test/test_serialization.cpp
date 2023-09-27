// test_serialization.cpp

#include "gtest/gtest.h"
#include "../BCS/Serialization.h"
#include "../BCS/Deserialization.h"
#include "../BCS/BCSTypes.h"
#include "../BCS/BCSMap.h"
#include "../BCS/U32.h"
#include "../BCS/BString.h"
#include "../BCS//Sequence.h"
#include "../BCS/Bool.h"
#include <cryptopp/integer.h>

TEST(SerializationTest, BoolSerialize) {
    Serialization s;
    s.SerializeBool(true);
    std::vector<uint8_t> res = s.GetBytes();

    std::vector<uint8_t> expected = {1};
    ASSERT_EQ(expected, res);
}

TEST(SerializationTest, BoolTrueDeserialize) {
    bool actual = true;
    Serialization s;
    s.SerializeBool(actual);
    std::vector<uint8_t> res = s.GetBytes();

    Deserialization d(res);
    bool expected = d.DeserializeBool();

    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, BoolFalseSerialize) {
    Serialization s;
    s.SerializeBool(false);
    std::vector<uint8_t> res = s.GetBytes();

    std::vector<uint8_t> expected = {0};
    ASSERT_EQ(expected, res);
}

TEST(SerializationTest, BoolFalseDeserialize) {
    bool expected = false;
    Serialization s;
    s.SerializeBool(expected);
    std::vector<uint8_t> res = s.GetBytes();

    Deserialization d(res);
    bool actual = d.DeserializeBool();

    ASSERT_EQ(expected, actual);
}

#include <cstring>

TEST(SerializationTest, ByteArraySerialize) {

    std::string value = "1234567890";

    std::vector<uint8_t> bytes(value.begin(), value.end());

    Serialization s;
    s.SerializeBytes(bytes);

    std::vector<uint8_t> actual = s.GetBytes();

    std::vector<uint8_t> expected = {10, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    ASSERT_EQ(expected, actual);

}

TEST(SerializationTest, ByteArrayDeserialize) {
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

TEST(SerializationTest, U8Serialize) {

    Serialization s;
    s.SerializeU8(123);

    std::vector<uint8_t> expected = {123};
    auto actual = s.GetBytes();

    ASSERT_EQ(expected, actual);

}

TEST(SerializationTest, U8Deserialize) {

    uint8_t expected = 123;

    Serialization s;
    s.SerializeU8(expected);

    std::vector<uint8_t> bytes = s.GetBytes();

    Deserialization d(bytes);
    uint8_t actual = d.DeserializeU8();

    ASSERT_EQ(expected, actual);

}

TEST(SerializationTest, U32Serialize) {

    uint32_t input = 57615782;

    Serialization s;
    s.SerializeU32(input);

    std::vector<uint8_t> expected = {166, 37, 111, 3};
    auto actual = s.GetBytes();

    ASSERT_EQ(expected, actual);

}

TEST(SerializationTest, U32Deserialize) {

    uint32_t expected = 57615782;

    Serialization s;
    s.SerializeU32(expected);

    std::vector<uint8_t> bytes = s.GetBytes();

    Deserialization d(bytes);
    uint32_t actual = d.DeserializeU32();

    ASSERT_EQ(expected, actual);

}


TEST(SerializationTest, U64Serialize) {

    uint64_t input = 9432012321182;

    Serialization s;
    s.SerializeU64(input);

    std::vector<uint8_t> expected = {158, 113, 190, 15, 148, 8, 0, 0};
    auto actual = s.GetBytes();

    ASSERT_EQ(expected, actual);

}

TEST(SerializationTest, U64Deserialize) {

    uint64_t expected = 9432012321182;

    Serialization s;
    s.SerializeU64(expected);

    std::vector<uint8_t> bytes = s.GetBytes();

    Deserialization d(bytes);
    uint64_t actual = d.DeserializeU64();

    ASSERT_EQ(expected, actual);

}

TEST(SerializationTest, U128Serialize) {

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

TEST(SerializationTest, U128Deserialize) {
    CryptoPP::Integer expected = 10;
    std::vector<uint8_t> bytes = Serialization().SerializeU128(expected).GetBytes();
    CryptoPP::Integer actual = Deserialization(bytes).DeserializeU128();
    ASSERT_EQ(expected, actual);

    expected = CryptoPP::Integer("749382032131231323910498053");
    bytes = Serialization().SerializeU128(expected).GetBytes();
    actual = Deserialization(bytes).DeserializeU128();
    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, U32AsUleb128Serialize) {
    std::vector<uint8_t> res = Serialization().SerializeU32AsUleb128(1160).GetBytes();
    std::vector<uint8_t> expected = {136, 9};
    ASSERT_EQ(expected, res);
}

TEST(SerializationTest, U32AsUleb128Deserialize) {
    uint32_t expected = 1160;
    std::vector<uint8_t> bytes = Serialization().SerializeU32AsUleb128(expected).GetBytes();
    uint32_t actual = Deserialization(bytes).DeserializeUleb128();
    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, StringSerialize) {
    std::vector<uint8_t> res = Serialization().SerializeString("potato UTF8: 🥔").GetBytes();
    std::vector<uint8_t> exp = { 17, 112, 111, 116, 97, 116, 111, 32, 85, 84, 70, 56, 58, 32, 240, 159, 165, 148 };
    ASSERT_EQ(exp, res);
}

TEST(SerializationTest, StringDeserialize) {
    std::string expected = "potato UTF8: 🥔";
    std::vector<uint8_t> bytes = Serialization().SerializeString(expected).GetBytes();
    std::string actual = Deserialization(bytes).DeserializeString();
    ASSERT_EQ(expected, actual);
}

TEST(SerializationTest, StringLongSerialize) {
    std::string longString = "potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔";
    std::vector<uint8_t> res = Serialization().SerializeString(longString).GetBytes();

    std::vector<uint8_t> exp = { 231, 2, 112, 111, 116, 97, 116, 111, 32, 85, 84, 70,
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
                                 148 };

    for (int i =0; i < exp.size(); i++){
        ASSERT_EQ(exp[i], res[i]);
    }
}

TEST(SerializationTest, StringLongDeserialize) {
    std::string expected = "potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔 potato UTF8: 🥔";
    std::vector<uint8_t> bytes = Serialization().SerializeString(expected).GetBytes();
    std::string actual = Deserialization(bytes).DeserializeString();

    ASSERT_EQ(expected, actual);
}

using MapType = std::map<BString, ISerializable*>;

TEST(SerializationTest, MapBStringU32Test) {
    MapType map;
    map[BString("a")] = new U32(12345);
    map[BString("b")] = new U32(99234);
    map[BString("c")] = new U32(23829);

    BCSMap bcsMap(map);
    Serialization ser;
    bcsMap.Serialize(ser);

    std::vector<uint8_t> serialized = ser.GetBytes();
    std::vector<uint8_t> expected = {3, 1, 97, 57, 48, 0, 0, 1, 98, 162, 131, 1, 0, 1, 99, 21, 93, 0, 0};

    EXPECT_EQ(expected, serialized);

    // Clean up
    for (auto& kv : map) {
        delete kv.second;
    }
}

TEST(SerializationTest, MapBStringU32TestDe) {
    MapType expectedMap;
    expectedMap[BString("x")] = new U32(12345);
    expectedMap[BString("b")] = new U32(99234);
    expectedMap[BString("c")] = new U32(23829);

    BCSMap expectedBcsMap(expectedMap);
    Serialization ser;
    expectedBcsMap.Serialize(ser);

    std::vector<uint8_t> bytes = ser.GetBytes();
    Deserialization deser(bytes);
    BCSMap actualBcsMap = deser.DeserializeMap(
            [](Deserialization& d) -> ISerializable* { return U32::Deserialize(d); });

    std::map<BString, ISerializable*>* values = reinterpret_cast<std::map<BString, ISerializable*>*>(actualBcsMap.GetValue());
    // Expect the keys and values to be the same, but in sorted order
    std::vector<BString> key;
    std::vector<ISerializable*> value;
    for(std::map<BString,ISerializable*>::iterator it = values->begin(); it != values->end(); ++it) {
        key.push_back(it->first);
        value.push_back(it->second);
    }
    EXPECT_TRUE(BString("b").Equals(key[0]));
    EXPECT_TRUE(BString("c").Equals(key[1]));
    EXPECT_TRUE(BString("x").Equals(key[2]));
    EXPECT_TRUE(U32(99234).Equals(*(U32*)value[0]));
    EXPECT_TRUE(U32(23829).Equals(*(U32*)value[1]));
    EXPECT_TRUE(U32(12345).Equals(*(U32*)value[2]));
}


TEST(SerializationTest, MapBStringU32Test1) {
    MapType map;
    map[BString("x")] = new U32(12345);
    map[BString("b")] = new U32(99234);
    map[BString("c")] = new U32(23829);

    BCSMap bcsMap(map);
    Serialization ser;
    bcsMap.Serialize(ser);

    std::vector<uint8_t> serialized = ser.GetBytes();
    std::vector<uint8_t> expected = {3, 1, 98, 162, 131, 1, 0, 1, 99, 21, 93, 0, 0, 1, 120, 57, 48, 0, 0};

    EXPECT_EQ(expected, serialized);

    // Clean up
    for (auto& kv : map) {
        delete kv.second;
    }
}

TEST(SerializationTest, MapBStringU32Test2) {
    MapType map;
    map[BString("b")] = new U32(12345);
    map[BString("x")] = new U32(99234);
    map[BString("c")] = new U32(23829);

    BCSMap bcsMap(map);
    Serialization ser;
    bcsMap.Serialize(ser);

    std::vector<uint8_t> serialized = ser.GetBytes();
    std::vector<uint8_t> expected = {3, 1, 98, 57, 48, 0, 0, 1, 99, 21, 93, 0, 0, 1, 120, 162, 131, 1, 0 };

    EXPECT_EQ(expected, serialized);

    // Clean up
    for (auto& kv : map) {
        delete kv.second;
    }
}


TEST(SerializationTest, MapBStringU32Test3) {
    MapType map;
    map[BString("b")] = new U32(99234);
    map[BString("x")] = new U32(12345);
    map[BString("c")] = new U32(23829);

    BCSMap bcsMap(map);
    Serialization ser;
    bcsMap.Serialize(ser);

    std::vector<uint8_t> serialized = ser.GetBytes();
    std::vector<uint8_t> expected = {3, 1, 98, 162, 131, 1, 0, 1, 99, 21, 93, 0, 0, 1, 120, 57, 48, 0, 0 };

    EXPECT_EQ(expected, serialized);

    // Clean up
    for (auto& kv : map) {
        delete kv.second;
    }
}

TEST(SerializationTest, MapBStringBStringTest) {
    MapType map;
    map[BString("a")] = new BString("a");
    map[BString("b")] = new BString("b");
    map[BString("c")] = new BString("c");

    BCSMap bcsMap(map);
    Serialization ser;
    bcsMap.Serialize(ser);

    std::vector<uint8_t> serialized = ser.GetBytes();
    std::vector<uint8_t> expected = {3, 1, 97, 1, 97, 1, 98, 1, 98, 1, 99, 1, 99 };

    EXPECT_EQ(expected, serialized);

    // Clean up
    for (auto& kv : map) {
        delete kv.second;
    }
}

TEST(SerializationTest, StringBytesEmptySerialize) {
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

TEST(SerializationTest, MultipleValues_Serialize) {
    Serialization serializer;
    serializer.Serialize(std::string("potato"));
    serializer.Serialize(static_cast<uint32_t>(123));
    serializer.Serialize(true);
    serializer.Serialize(static_cast<uint32_t>(456));
    std::vector<uint8_t> res = serializer.GetBytes();
    std::vector<uint8_t> exp = { 6, 112, 111, 116, 97, 116, 111, 123, 0, 0, 0, 1, 200, 1, 0, 0 };
    EXPECT_EQ(exp, res);
}

TEST(SerializationTest, SequenceBStringEmptySerialize) {
    Serialization ser;
    std::vector<ISerializable*> strArr = { new BString("") };
    Sequence seq(strArr);

    seq.Serialize(ser);

    std::vector<uint8_t> actual = ser.GetBytes();
    std::vector<uint8_t> expected = { 1, 1, 0 };
    EXPECT_EQ(expected, actual);
}

TEST(SerializationTest, SequenceBStringLongDeserialize) {
Serialization ser;
std::vector<ISerializable*> expectedStrArr = { new BString("a"), new BString("abc"),
                                               new BString("def"), new BString("ghi") };
ser.Serialize(expectedStrArr);

Sequence expectedSeq(expectedStrArr);

std::vector<uint8_t> actual = ser.GetBytes();
std::vector<uint8_t> exp = { 4, 1, 97, 3, 97, 98, 99, 3, 100, 101, 102, 3, 103, 104, 105 };
EXPECT_EQ(exp, actual);

Deserialization deser(actual);
std::vector<ISerializable*> actualSequenceArr = deser.DeserializeSequence(
        [](Deserialization& d) -> BString* { return BString::Deserialize(d); });

for (int i = 0; i < expectedStrArr.size(); i++){
    EXPECT_EQ(expectedStrArr[i]->ToString(), actualSequenceArr[i]->ToString());
}
}


TEST(SerializationTest, SequenceBStringSerialize) {
    Serialization ser;
    std::vector<ISerializable*> expectedStrArr = { new BString("a"), new BString("abc"),
                                                   new BString("def"), new BString("ghi") };
    ser.Serialize(expectedStrArr);

    std::vector<uint8_t> actual = ser.GetBytes();
    std::vector<uint8_t> exp = { 4, 1, 97, 3, 97, 98, 99, 3, 100, 101, 102, 3, 103, 104, 105 };
    EXPECT_EQ(exp, actual);
}

TEST(SerializationTest, SequenceBoolSerialize) {
    Serialization ser;
    std::vector<ISerializable*> expectedStrArr = { new Bool(false), new Bool(true),
                                                   new Bool(false)};
    ser.Serialize(expectedStrArr);

    std::vector<uint8_t> actual = ser.GetBytes();
    std::vector<uint8_t> exp = { 3, 0, 1, 0 };
    EXPECT_EQ(exp, actual);
}

TEST(SerializationTest, SequenceBoolDeserializer) {
    Serialization ser;
    std::vector<ISerializable*> expectedStrArr = { new Bool(false), new Bool(true),
                                                   new Bool(false)};
    ser.Serialize(expectedStrArr);

    std::vector<uint8_t> expectedByteArr = ser.GetBytes();
    std::vector<uint8_t> actualByteArr = { 3, 0, 1, 0 };
    EXPECT_EQ(expectedByteArr, actualByteArr);

    Deserialization deser(expectedByteArr);
    std::vector<ISerializable*> actualSequenceArr = deser.DeserializeSequence(
            [](Deserialization& d) -> Bool* { return Bool::Deserialize(d); });

    EXPECT_EQ(expectedStrArr[0]->ToString(), actualSequenceArr[0]->ToString());
    EXPECT_EQ(expectedStrArr[1]->ToString(), actualSequenceArr[1]->ToString());
    EXPECT_EQ(expectedStrArr[2]->ToString(), actualSequenceArr[2]->ToString());
    EXPECT_EQ(expectedStrArr[3]->ToString(), actualSequenceArr[3]->ToString());
}