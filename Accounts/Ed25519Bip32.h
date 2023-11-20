#ifndef ED25519BIP32_H
#define ED25519BIP32_H
#if defined(__APPLE__)
namespace std
{
    template <class Arg, class Result>
    struct unary_function
    {
        typedef Arg argument_type;
        typedef Result result_type;
    };

    template <class Arg1, class Arg2, class Result>
    struct binary_function
    {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };
}
#endif
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>

namespace Aptos::Accounts
{
    class Ed25519Bip32
    {
    private:
        static const std::string Curve;
        static const uint32_t HardenedOffset;

        CryptoPP::SecByteBlock _masterKey;
        CryptoPP::SecByteBlock _chainCode;

        static std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> GetMasterKeyFromSeed(const CryptoPP::SecByteBlock &seed);
        static std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> GetChildKeyDerivation(const CryptoPP::SecByteBlock &key,
                                                                                               const CryptoPP::SecByteBlock &chainCode,
                                                                                               uint32_t index);
        static std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> HmacSha512(const CryptoPP::SecByteBlock &keyBuffer,
                                                                                    const CryptoPP::SecByteBlock &data);
        static bool IsValidPath(const std::string &path);

    public:
        Ed25519Bip32(const std::vector<uint8_t> &seed);
        std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> DerivePath(const std::string &path);
    };
}
#endif // ED25519BIP32_H
