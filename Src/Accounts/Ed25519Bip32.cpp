#include "Ed25519Bip32.h"
#include "../HDWallet/Utils/Utils.h"

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <bit>
// Additional Windows-specific includes and code
#pragma comment(lib, "Ws2_32.lib")
#else
#include <netinet/in.h>
#endif
#include <regex>

namespace Aptos::Accounts
{
    const std::string Ed25519Bip32::Curve = "ed25519 seed";
    const uint32_t Ed25519Bip32::HardenedOffset = 0x80000000;

    std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> Ed25519Bip32::DerivePath(const std::string &path)
    {
        if (!IsValidPath(path))
        {
            throw std::invalid_argument("Invalid derivation path");
        }

        std::istringstream stream(path);
        std::string segment;
        getline(stream, segment, '/'); // Read and discard the "m"

        std::vector<uint32_t> segments;
        while (getline(stream, segment, '/'))
        {
            uint32_t index = std::stoul(segment);
            if (segment.back() == '\'')
            {
                index += HardenedOffset;
            }
            segments.push_back(index);
        }

        std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> result = std::make_pair(_masterKey, _chainCode);
        for (uint32_t index : segments)
        {
            result = GetChildKeyDerivation(result.first, result.second, index);
        }

        return result;
    }

    bool Ed25519Bip32::IsValidPath(const std::string &path)
    {
        if (std::regex regex("^m(\\/[0-9]+')+$"); !std::regex_match(path, regex))
        {
            return false;
        }
        return true;
    }

    std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> Ed25519Bip32::HmacSha512(const CryptoPP::SecByteBlock &keyBuffer, const CryptoPP::SecByteBlock &data)
    {
        CryptoPP::HMAC<CryptoPP::SHA512> hmac(keyBuffer, keyBuffer.size());
        hmac.Update(data, data.size());
        CryptoPP::SecByteBlock i(64);
        hmac.Final(i);
        CryptoPP::SecByteBlock il(32);
        il.Assign(i.data(), 32);
        CryptoPP::SecByteBlock ir(32);
        ir.Assign(i.data() + 32, 32);
        return std::make_pair(il, ir);
    }

    Ed25519Bip32::Ed25519Bip32(const std::vector<uint8_t> &seed)
    {
        _masterKey.resize(32);
        _chainCode.resize(32);

        std::tie(_masterKey, _chainCode) = GetMasterKeyFromSeed(CryptoPP::SecByteBlock(seed.data(), seed.size()));
    }

    std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> Ed25519Bip32::GetMasterKeyFromSeed(const CryptoPP::SecByteBlock &seed)
    {
        return HmacSha512(Utils::StringToSecByteBlock(Curve), seed);
    }

    std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> Ed25519Bip32::GetChildKeyDerivation(const CryptoPP::SecByteBlock &key,
                                                                                                  const CryptoPP::SecByteBlock &chainCode,
                                                                                                  uint32_t index)
    {
        std::vector<uint8_t> buffer;
        buffer.push_back(0);
        buffer.insert(buffer.end(), key.begin(), key.end());
       
        // Convert index to big-endian
        index = htonl(index);
        auto indexPtr = std::bit_cast<uint8_t*>(&index);

        // Insert index into buffer
        buffer.insert(buffer.end(), indexPtr, indexPtr + sizeof(index));

        return HmacSha512(chainCode, CryptoPP::SecByteBlock(buffer.data(), buffer.size()));
    }
}
