#include "gtest/gtest.h"
#include "../HDWallet/Utils/Utils.h"
#include <string>

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
