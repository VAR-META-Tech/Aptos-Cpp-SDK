//
// Created by Anh NPH on 25/09/2023.
//

#ifndef APTOS_UTILS_H
#define APTOS_UTILS_H

#include <regex>
#include <map>
#include <vector>
#include <string>

namespace Utils {
    bool IsValidAddress(std::string walletAddress);
    std::vector<uint8_t> ByteArrayFromHexString(std::string input);
    std::string HexStringFromByteArray(const std::vector<uint8_t>& input);

    template<typename TKey, typename TValue>
    void AddOrReplace(std::map<TKey, TValue>& dictionary, TKey key, TValue value);

    template<typename TKey, typename TValue>
    TValue TryGet(const std::map<TKey, TValue>& dictionary, TKey key);

    template <typename T>
    std::vector<T> Slice(std::vector<T> source, int start, int end);
    template <typename T>
    std::vector<T> Slice(std::vector<T> source, int start);

    std::vector<uint8_t> Sha256(const std::vector<uint8_t>& data, int offset, int count);
    std::vector<uint8_t> Sha256(const std::vector<uint8_t>& data);

    std::pair<std::vector<uint8_t>, std::vector<uint8_t>> EdKeyPairFromSeed(const std::vector<uint8_t>& seed);
}

#endif //APTOS_UTILS_H
