#include "Utils.h"
#include <algorithm>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/xed25519.h>
#include <sstream>
#include <iomanip>
#include "../../Accounts/Ed25519/PrivateKey.h"

using namespace Aptos::Accounts;
using namespace Aptos::Accounts::Ed25519;

namespace Aptos::Utils
{
    bool IsValidAddress(std::string walletAddress)
    {
        if (walletAddress.starts_with("0x"))
            walletAddress = walletAddress.substr(2);

        std::regex pattern("[a-fA-F0-9]{64}$");
        return std::regex_match(walletAddress, pattern);
    }

    std::vector<uint8_t> ByteArrayFromHexString(std::string input)
    {
        if (input.starts_with("0x"))
            input = input.substr(2);

        size_t outputLength = input.length() / 2;
        std::vector<uint8_t> output(outputLength);

        for (int i = 0; i < outputLength; i++)
            output[i] = std::stoi(input.substr(i * 2, 2), nullptr, 16);

        return output;
    }

    std::string HexStringFromByteArray(const std::vector<uint8_t> &input)
    {
        std::string output;
        for (const auto &byte : input)
        {
            char buf[3];
            snprintf(buf, sizeof(buf), "%02x", byte);
            output += buf;
        }
        return output;
    }

    template <typename TKey, typename TValue>
    void addOrReplace(std::map<TKey, TValue> &dictionary, TKey key,
                      TValue value)
    {
        dictionary[key] =
            value;
    }

    template <typename TKey, typename TValue>
    TValue TryGet(const std::map<TKey, TValue> &dictionary, TKey key)
    {
        auto it = dictionary.find(key);
        if (it != dictionary.end())
            return it->second;
        else
            return TValue(); // Or whatever default value you want
    }

    template <typename T>
    std::vector<T> Slice(std::vector<T> source, int start, int end)
    {
        if (end < 0)
            end = source.size();

        int len = end - start;

        std::vector<T> res(source.begin() + start, source.begin() + end);
        return res;
    }

    template <typename T>
    std::vector<T> Slice(std::vector<T> source, int start)
    {
        return Slice(source, start, -1);
    }

    std::vector<uint8_t> Sha256(const std::vector<uint8_t> &data, int offset, int count)
    {
        CryptoPP::SHA256 hash;
        std::vector<uint8_t> digest(hash.DigestSize());

        std::string message(data.begin() + offset, data.begin() + offset + count);
        hash.CalculateDigest(digest.data(), (unsigned char *)message.data(), message.size());

        return digest;
    }

    std::vector<uint8_t> Sha256(const std::vector<uint8_t> &data)
    {
        return Sha256(data, 0, data.size());
    }

    std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> EdKeyPairFromSeed(const CryptoPP::SecByteBlock &seed)
    {
        PrivateKey priv(seed);
        auto pub = priv.GetPublicKey();
        return {seed, pub.KeyBytes()};
    }

    CryptoPP::SecByteBlock ByteVectorToSecBlock(const std::vector<uint8_t> &input)
    {
        CryptoPP::SecByteBlock output(input.data(), input.size());
        return output;
    }

    std::vector<uint8_t> SecBlockToByteVector(const CryptoPP::SecByteBlock &input)
    {
        std::vector<uint8_t> byteArray(input.data(), input.data() + input.size());
        return byteArray;
    }

    CryptoPP::SecByteBlock StringToSecByteBlock(const std::string &str)
    {
        CryptoPP::SecByteBlock secBlock(str.size());
        std::memcpy(secBlock.BytePtr(), str.c_str(), str.size());
        return secBlock;
    }

    std::string ltrim(const std::string &s, const std::string &charsToTrim)
    {
        auto it = std::find_if(s.begin(), s.end(), [&charsToTrim](char c)
                               { return charsToTrim.find(c) == std::string::npos; });
        return std::string(it, s.end());
    }

    std::string rtrim(const std::string &s, const std::string &charsToTrim)
    {
        auto it = std::find_if(s.rbegin(), s.rend(), [&charsToTrim](char c)
                               { return charsToTrim.find(c) == std::string::npos; });
        return std::string(s.begin(), it.base());
    }

    std::string trim(const std::string &s, const std::string &charsToTrim)
    {
        return rtrim(ltrim(s, charsToTrim), charsToTrim);
    }

    std::string floatToStringWithFormat(float value, int precision)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << value;
        return stream.str();
    }

}
