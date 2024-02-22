#include "gtest/gtest.h"
#include "../HDWallet/Utils/Utils.h"
#include <string>

using namespace Aptos;
class UtilsTest : public ::testing::Test {
protected:
    const std::string PrivateKeyHex = "0x64f57603b58af16907c18a866123286e1cbce89790613558dc1775abb3fc5c8c";
    const std::string PrivateKeyHexTrimmed = "64f57603b58af16907c18a866123286e1cbce89790613558dc1775abb3fc5c8c";
    const std::string InvalidPrivateKeyLengthHexOne = "0x64f57603b58af16907c18a866123286e1cbce89790613558dc1775abb3fc5c8c98ykj";
    const std::string InvalidPrivateKeyLengthHexTwo = "0x64f57603b58af16907c18a866123286e1cbce89790613558dc1775abb3";
    const std::string InvalidCharactersPrivateKeyHex = "0x64f57603b58af16907c18a8|3286e1cbce89790613558dc1775abb3fc5c8c";
};

TEST_F(UtilsTest, IsValidHexAddressTrue) {
    ASSERT_TRUE(Utils::IsValidAddress(PrivateKeyHex));
}

TEST_F(UtilsTest, IsValidTrimmedHexAddressTrue) {
    ASSERT_TRUE(Utils::IsValidAddress(PrivateKeyHexTrimmed));
}

TEST_F(UtilsTest, IsInvalidLengthHexAddress) {
    ASSERT_FALSE(Utils::IsValidAddress(InvalidPrivateKeyLengthHexOne));
}

TEST_F(UtilsTest, IsInvalidShorterLengthHexAddress) {
    ASSERT_FALSE(Utils::IsValidAddress(InvalidPrivateKeyLengthHexTwo));
}

TEST_F(UtilsTest, IsInvalidCharacterHexAddress) {
    ASSERT_FALSE(Utils::IsValidAddress(InvalidPrivateKeyLengthHexTwo));
}

TEST_F(UtilsTest, Trim_RemovesCharactersFromBothEnds) {
    // Test trimming spaces
    std::string original = "   hello world   ";
    std::string trimmed = Utils::trim(original, " ");
    ASSERT_EQ(trimmed, "hello world");

    // Test trimming specific characters
    original = "xxxyyyzzz";
    trimmed = Utils::trim(original, "xyz");
    ASSERT_EQ(trimmed, "");

    // Test trimming when no characters should be trimmed
    original = "hello world";
    trimmed = Utils::trim(original, "xyz");
    ASSERT_EQ(trimmed, "hello world");

    // Test trimming empty string
    original = "";
    trimmed = Utils::trim(original, " ");
    ASSERT_EQ(trimmed, "");

    // Test trimming spaces and special characters
    original = " $% hello world %$ ";
    trimmed = Utils::trim(original, " $%");
    ASSERT_EQ(trimmed, "hello world");
}

TEST_F(UtilsTest, RTrim_RemovesCharactersFromRightEnd) {
    // Test trimming spaces
    std::string original = "   hello world   ";
    std::string trimmed = Utils::rtrim(original, " ");
    ASSERT_EQ(trimmed, "   hello world");

    // Test trimming specific characters
    original = "xxxyyyzzz";
    trimmed = Utils::rtrim(original, "xyz");
    ASSERT_EQ(trimmed, "");

    // Test trimming when no characters should be trimmed
    original = "hello world";
    trimmed = Utils::rtrim(original, "xyz");
    ASSERT_EQ(trimmed, "hello world");

    // Test trimming empty string
    original = "";
    trimmed = Utils::rtrim(original, " ");
    ASSERT_EQ(trimmed, "");

    // Test trimming spaces and special characters
    original = " $% hello world %$ ";
    trimmed = Utils::rtrim(original, " $%");
    ASSERT_EQ(trimmed, " $% hello world");
}