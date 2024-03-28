//
// Created by Anh NPH on 26/09/2023.
//

#ifndef APTOS_PRIVATEKEY_H
#define APTOS_PRIVATEKEY_H

#include <string>
#include <vector>
#include "../Ed25519Signature.h"
#include "PublicKey.h"
#include "../../BCS/BCS.h"
#include <stdexcept>
#include <cryptopp/xed25519.h>

namespace Aptos::Accounts::Ed25519
{
    /// <summary>
    /// Represents a 64-byte extended private key.
    /// An extended private key is a requirement from Choas.NaCl.
    ///
    /// Note that the hexadecimal string representation is of the 32-byte private key on it's own.
    /// </summary>
    class APTOS_API PrivateKey : public CryptoPP::ed25519PrivateKey
    {
    public:
        /// <summary>
        /// Extended private key length.
        /// </summary>
        static const int ExtendedKeyLength = 64;

        /// <summary>
        /// Private key length.
        /// </summary>
        static const int KeyLength = 32;

        /// <summary>
        /// The key as a 32-byte hexadecimal string (64 characters).
        /// </summary>
        std::string Key();
        void Key(std::string const &key);

        /// <summary>
        /// The 32-byte private key in bytes.
        /// Checks if we have the 32-byte private key or 64-byte extended key,
        /// otherwise uses the string representation to create both.
        /// </summary>
        CryptoPP::SecByteBlock KeyBytes();
        void KeyBytes(CryptoPP::SecByteBlock value);

        /// <summary>
        /// Initializes the PrivateKey object with a 64 byte array that represents the expanded private key from seed.
        /// For example, using: <c>Ed25519.ExpandedPrivateKeyFromSeed(seed)</c>.
        /// This constructor is expected to be called from the <see cref="Account.Account()">Account</see> constructor.
        /// Note: To create a private key from a 32-byte string see <see cref="PrivateKey(string key)">PrivateKey(string key)</see>
        /// </summary>
        /// <param name="privateKey">64-byte array representation of the private key.</param>
        explicit PrivateKey(CryptoPP::SecByteBlock privateKey);

        /// <summary>
        /// Initializes the PrivateKey object with a 64 character (32-byte) ASCII representation of a private key.
        /// Note: The undelying cryptographic library (Chaos.NaCL) uses an extended private key (64 byte) for fast computation.
        /// This hex string is used as a seed to create an extended private key when <see cref="KeyBytes">KeyBytes</see> is requested.
        /// </summary>
        /// <param name="key">The private key as an ASCII encoded string.
        /// Example: <c>0x64f57603b58af16907c18a866123286e1cbce89790613558dc1775abb3fc5c8c</c></param>
        explicit PrivateKey(std::string const &key);

        /// <summary>
        /// Initialize the PrivateKey object from the given string.
        /// </summary>
        /// <param name="key">The private key as a hex encoded byte array.</param>
        explicit PrivateKey(std::array<CryptoPP::byte, KeyLength> privateKey);

        /// <summary>
        /// Create a private key from a string literal.
        /// Conforms to the standard Python and Typescript Aptos SDK.
        /// </summary>
        /// <param name="key">The private key as an ASCII encoded string.</param>
        /// <returns>Private key object.</returns>
        static PrivateKey FromHex(std::string key);

        /// <summary>
        /// Derives public key from the private key bytes.
        /// </summary>
        /// <returns>PublicKey object.</returns>
        PublicKey GetPublicKey();
        static PrivateKey Random();

        /// <summary>
        /// Compartor for two private keys.
        /// </summary>
        /// <param name="lhs">First private key in comparison..</param>
        /// <param name="rhs">Second private key in comparison.</param>
        /// <returns></returns>
        bool operator==(const PrivateKey &rhs) const;

        /// <summary>
        /// Sign a message using the extended private key.
        /// </summary>
        /// <param name="message">The message to sign, represented in bytes.</param>
        /// <returns>The signature generated for the message as an object</returns>
        Ed25519Signature Sign(CryptoPP::SecByteBlock message);

        /// <summary>
        /// Serialize private key
        /// </summary>
        /// <param name="serializer">Serializer object</param>
        void Serialize(BCS::Serialization &serializer);

        /// <inheritdoc cref="Equals(object)"/>
        bool Equals(const PrivateKey &rhs) const;

        /// <inheritdoc cref="GetHashCode"/>
        int GetHashCode() const;

        /// <inheritdoc cref="ToString"/>
        std::string ToString();

    private:
        /// <summary>
        /// Hex string representation of private key.
        /// </summary>
        std::string _key;

        /// <summary>
        /// The 64-byte extended private key.
        /// This key is used internally for signing.
        /// A public accessor that returns a 32-byte private is found in <see cref="_keyBytes">KeyBytes</see>
        /// </summary>
        CryptoPP::SecByteBlock _extendedKeyBytes;

        /// <summary>
        /// The 32-byte private key
        /// This key is exposed publicly by <see cref="KeyBytes">KeyBytes</see>
        /// </summary>
        CryptoPP::SecByteBlock _keyBytes;

        void GenerateExtendedKey();
    };
}

#endif // APTOS_PRIVATEKEY_H
