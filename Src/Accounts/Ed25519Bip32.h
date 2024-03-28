#ifndef ED25519BIP32_H
#define ED25519BIP32_H
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include "BCS/BCSTypes.h"

namespace Aptos::Accounts
{
    /// <summary>
    /// An implementation of Ed25519 based BIP32 key generation.
    /// </summary>
    class APTOS_API Ed25519Bip32
    {
    private:
        /// <summary>
        /// The seed for the Ed25519 BIP32 HMAC-SHA512 master key calculation.
        /// </summary>
        static const std::string Curve;

        /// <summary>
        /// Hardened offset for BIP32 keys, commonly 0x80000000. Used to create secure, hardened child keys.
        /// </summary>
        static const uint32_t HardenedOffset;

        /// <summary>
        /// The computed master key.
        /// </summary>
        CryptoPP::SecByteBlock _masterKey;

        /// <summary>
        /// The computed chain code.
        /// </summary>
        CryptoPP::SecByteBlock _chainCode;

        /// <summary>
        /// Gets the master key used for key generation from the passed seed.
        /// </summary>
        /// <param name="seed">The seed used to calculate the master key.</param>
        /// <returns>A tuple consisting of the key and corresponding chain code.</returns>
        static std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> GetMasterKeyFromSeed(const CryptoPP::SecByteBlock &seed);

        /// <summary>
        /// Computes the child key.
        /// </summary>
        /// <param name="key">The key used to derive from.</param>
        /// <param name="chainCode">The chain code for derivation.</param>
        /// <param name="index">The index of the key to the derive.</param>
        /// <returns>A tuple consisting of the key and corresponding chain code.</returns>
        static std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> GetChildKeyDerivation(const CryptoPP::SecByteBlock &key,
                                                                                               const CryptoPP::SecByteBlock &chainCode,
                                                                                               uint32_t index);

        /// <summary>
        /// Computes the HMAC SHA 512 of the byte array passed into <c>data</c>.
        /// </summary>
        /// <param name="keyBuffer">The byte array to be used as the HMAC SHA512 key.</param>
        /// <param name="data">The data to calculate the HMAC SHA512 on.</param>
        /// <returns>A tuple consisting of the key and corresponding chain code.</returns>
        static std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> HmacSha512(const CryptoPP::SecByteBlock &keyBuffer,
                                                                                    const CryptoPP::SecByteBlock &data);

    public:
        /// <summary>
        /// Initialize the ed25519 based bip32 key generator with the passed seed.
        /// </summary>
        /// <param name="seed">The seed.</param>
        explicit Ed25519Bip32(const std::vector<uint8_t> &seed);

        /// <summary>
        /// Derives a child key from the passed derivation path.
        /// </summary>
        /// <param name="path">The derivation path.</param>
        /// <returns>The key and chaincode.</returns>
        /// <exception cref="FormatException">Thrown when the passed derivation path is invalid.</exception>
        std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> DerivePath(const std::string &path);

        /// <summary>
        /// Checks if the derivation path is valid.
        /// <remarks>Returns true if the path is valid, otherwise false.</remarks>
        /// </summary>
        /// <param name="path">The derivation path.</param>
        /// <returns>A boolean.</returns>
        static bool IsValidPath(const std::string& path);
    };
}
#endif // ED25519BIP32_H
