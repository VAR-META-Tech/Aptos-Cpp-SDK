//
// Created by Anh NPH on 25/09/2023.
//

#include "Utils.h"
#include <algorithm>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/xed25519.h>

namespace Utils {
    bool IsValidAddress(std::string walletAddress) {
        if (walletAddress.substr(0, 2) == "0x")
            walletAddress = walletAddress.substr(2);

        std::regex pattern("[a-fA-F0-9]{64}$");
        return std::regex_match(walletAddress, pattern);
    }

    std::vector<uint8_t> ByteArrayFromHexString(std::string input) {
        if (input.substr(0, 2) == "0x")
            input = input.substr(2);

        size_t outputLength = input.length() / 2;
        std::vector<uint8_t> output(outputLength);

        for (int i = 0; i < outputLength; i++)
            output[i] = std::stoi(input.substr(i * 2, 2), nullptr, 16);

        return output;
    }

    std::string HexStringFromByteArray(const std::vector<uint8_t> &input) {
        std::string output;
        for (const auto &byte: input) {
            char buf[3];
            snprintf(buf, sizeof(buf), "%02x", byte);
            output += buf;
        }
        return output;
    }

    template<typename TKey, typename TValue>
    void addOrReplace(std::map < TKey, TValue > &dictionary, TKey
    key,
    TValue value
    ) {
    dictionary[key] =
    value;
    }


    template<typename TKey, typename TValue>
    TValue TryGet(const std::map<TKey, TValue> &dictionary, TKey key) {
        auto it = dictionary.find(key);
        if (it != dictionary.end())
            return it->second;
        else
            return TValue();  // Or whatever default value you want
    }

    template<typename T>
    std::vector<T> Slice(std::vector<T> source, int start, int end) {
        if (end < 0)
            end = source.size();

        int len = end - start;

        std::vector<T> res(source.begin() + start, source.begin() + end);
        return res;
    }

    template<typename T>
    std::vector<T> Slice(std::vector<T> source, int start) {
        return Slice(source, start, -1);
    }

    std::vector<uint8_t> Sha256(const std::vector<uint8_t>& data, int offset, int count) {
        CryptoPP::SHA256 hash;
        std::vector<uint8_t> digest(hash.DigestSize());

        std::string message(data.begin() + offset, data.begin() + offset + count);
        hash.CalculateDigest(digest.data(), (unsigned char*)message.data(), message.size());

        return digest;
    }

    std::vector<uint8_t> Sha256(const std::vector<uint8_t>& data) {
        return Sha256(data, 0, data.size());
    }

    std::pair<std::vector<uint8_t>, std::vector<uint8_t>> EdKeyPairFromSeed(const std::vector<uint8_t>& seed) {
        //todo
        std::vector<uint8_t> privateKey(32);
        std::vector<uint8_t> publicKey(32);
        return {privateKey, publicKey};
    }
}