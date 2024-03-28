//
// Created by Anh NPH on 25/09/2023.
//

#ifndef APTOS_PUBLICKEY_H
#define APTOS_PUBLICKEY_H
#include <string>
#include <vector>
#include <stdexcept>
#include "../Ed25519Signature.h"

namespace Aptos::Accounts::Ed25519
{
    /// <summary>
    /// Represents a 32-byte public key.
    /// </summary>
    class APTOS_API PublicKey : public BCS::ISerializable
    {
        /// <summary>
        /// Public key length.
        /// </summary>
    public:
        static const int KeyLength = 32;

        /// <summary>
        /// Byte representation of public key.
        /// </summary>
    private:
        CryptoPP::SecByteBlock _keyBytes;

    public:
        /// <summary>
        /// Initializes the PublicKey object with a given byte array.
        /// </summary>
        /// <param name="publicKey">The public key as byte array.</param>
        explicit PublicKey(const CryptoPP::SecByteBlock &publicKey);

        /// <summary>
        /// Initializes the PublicKey object with a given hexadecimal representation of public .
        /// </summary>
        /// <param name="key">The public key as a hexadecimal string.
        /// Example: <c>0x586e3c8d447d7679222e139033e3820235e33da5091e9b0bb8f1a112cf0c8ff5</c>
        /// </param>
        explicit PublicKey(std::string key);

        /// <summary>
        /// The key as a hexadecimal string
        /// </summary>
        std::string Key() const;
        void setKey(std::string key);

        /// <summary>
        /// The key in bytes.
        /// </summary>
        CryptoPP::SecByteBlock KeyBytes() const;
        void setKeyBytes(const CryptoPP::SecByteBlock &bytes);

        /// <summary>
        /// Verify a signed message using the current public key.
        /// </summary>
        /// <param name="message">Message that was signed.</param>
        /// <param name="signature">The signature from the message.</param>
        /// <returns></returns>
        bool Verify(const CryptoPP::SecByteBlock &message, const Ed25519Signature &signature) const;

        /// <summary>
        /// Check if PubliKey is a valid on the Ed25519 curve.
        /// </summary>
        /// <returns>Returns true if public key is on the curve.</returns>
        bool IsOnCurve() const;

        /// <summary>
        /// Serialize public key
        /// </summary>
        /// <param name="serializer">Serializer object</param>
        void Serialize(BCS::Serialization &serializer) const override;
        static std::shared_ptr<BCS::ISerializable> Deserialize(BCS::Deserialization &deserializer);

        /// <summary>
        /// ToString implementation return the key as a hex string.
        /// </summary>
        /// <returns></returns>
        std::string ToString() const override;

        /// <summary>
        /// Value used as a hash
        /// </summary>
        /// <returns></returns>
        size_t GetHashCode() const override;

        /// <inheritdoc cref="object.Equals(object)"/>
        bool Equals(const PublicKey &rhs) const;

        bool operator==(const PublicKey &rhs) const;
    };
}

#endif // APTOS_PUBLICKEY_H
