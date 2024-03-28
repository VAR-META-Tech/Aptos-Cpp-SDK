#ifndef APTOS_UTILS_H
#define APTOS_UTILS_H
#include <regex>
#include <map>
#include <vector>
#include <string>
#include <cstdint>
#include <cryptopp/hex.h>
#include "BCS/BCSTypes.h"

/// <summary>
/// Implements utility methods to be used in the wallet.
/// </summary>
namespace Aptos::Utils
{
    constexpr int Ed25519PrivateKeySeedSizeInBytes = 32;
    constexpr int Ed25519PublicKeySizeInBytes = 32;

    /// <summary>
    /// Check if it's a valid hex address.
    /// </summary>
    /// <param name="walletAddress"></param>
    /// <returns>true if is a valid hex address, false otherwise.</returns>
    bool APTOS_API IsValidAddress(std::string walletAddress);

    /// <summary>
    /// Converts a hexadecimal string to an array of bytes
    /// NOTE: string must not contain "0x"
    /// Wrong input:   0x586e3c8d447d7679222e139033e3820235e33da5091e9b0bb8f1a112cf0c8ff5
    /// Correct input: 586e3c8d447d7679222e139033e3820235e33da5091e9b0bb8f1a112cf0c8ff5
    /// </summary>
    /// <param name="input"></param> Valid hexadecimal string
    /// <returns>Byte array representation of hexadecimal string</returns>
    std::vector<uint8_t> APTOS_API ByteArrayFromHexString(std::string input);

    /// <summary>
    /// Turn byte array to hex string without 0x identifier
    /// </summary>
    /// <param name="input"></param>
    /// <returns></returns>
    std::string APTOS_API HexStringFromByteArray(const std::vector<uint8_t> &input);

    template <typename TKey, typename TValue>
    /// <summary>
    /// Adds or replaces a value in a dictionary.
    /// </summary>
    /// <param name="dictionary">The dictionary.</param>
    /// <param name="key">The key to add or replace.</param>
    /// <param name="value">The value.</param>
    /// <typeparam name="TKey">The type of the key.</typeparam>
    /// <typeparam name="TValue">The type of the value.</typeparam>
    void APTOS_API AddOrReplace(std::map<TKey, TValue> &dictionary, TKey key, TValue value);

    template <typename TKey, typename TValue>
    /// <summary>
    /// Attempts to get a value from a dictionary.
    /// </summary>
    /// <param name="dictionary">The dictionary to get the value from.</param>
    /// <param name="key">The key to get.</param>
    /// <typeparam name="TKey">The type of the key.</typeparam>
    /// <typeparam name="TValue">The type of the value.</typeparam>
    /// <returns>The value.</returns>
    TValue APTOS_API TryGet(const std::map<TKey, TValue> &dictionary, TKey key);

    template <typename T>
    /// <summary>
    /// Slices the array, returning a new array starting at <c>start</c> index and ending at <c>end</c> index.
    /// </summary>
    /// <param name="source">The array to slice.</param>
    /// <param name="start">The starting index of the slicing.</param>
    /// <param name="end">The ending index of the slicing.</param>
    /// <typeparam name="T">The array type.</typeparam>
    /// <returns>The sliced array.</returns>
    std::vector<T> APTOS_API Slice(std::vector<T> source, int start, int end);

    template <typename T>
    /// <summary>
    /// Slices the array, returning a new array starting at <c>start</c>.
    /// </summary>
    /// <param name="source">The array to slice.</param>
    /// <param name="start">The starting index of the slicing.</param>
    /// <typeparam name="T">The array type.</typeparam>
    /// <returns>The sliced array.</returns>
    std::vector<T> APTOS_API Slice(std::vector<T> source, int start);

    /// <summary>
    /// Calculates the Sha256 of the given data.
    /// </summary>
    /// <param name="data">The data to hash.</param>
    /// <returns>The hash.</returns>
    std::vector<uint8_t> APTOS_API Sha256(const std::vector<uint8_t> &data, int offset, int count);

    /// <summary>
    /// Calculates the SHA256 of the given data.
    /// </summary>
    /// <param name="data">The data to hash.</param>
    /// <param name="offset">The offset at which to start.</param>
    /// <param name="count">The number of bytes to in the array to use as data.</param>
    /// <returns>The hash.</returns>
    std::vector<uint8_t> APTOS_API Sha256(const std::vector<uint8_t> &data);

    /// <summary>
    /// Gets the corresponding ed25519 key pair from the passed seed.
    /// </summary>
    /// <param name="seed">The seed</param>
    /// <returns>The key pair.</returns>
    std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> APTOS_API EdKeyPairFromSeed(const CryptoPP::SecByteBlock &seed);
    CryptoPP::SecByteBlock APTOS_API ByteVectorToSecBlock(const std::vector<uint8_t> &input);
    std::vector<uint8_t> APTOS_API SecBlockToByteVector(const CryptoPP::SecByteBlock &input);
    CryptoPP::SecByteBlock APTOS_API StringToSecByteBlock(const std::string &str);
    // Function to trim specified characters from the left side of a string
    std::string APTOS_API ltrim(const std::string &s, const std::string &charsToTrim);
    // Function to trim specified characters from the right side of a string
    std::string APTOS_API rtrim(const std::string &s, const std::string &charsToTrim);
    // Function to trim specified characters from both sides of a string
    std::string APTOS_API trim(const std::string &s, const std::string &charsToTrim);
    std::string APTOS_API floatToStringWithFormat(float value, int precision);
}
#endif // APTOS_UTILS_H
