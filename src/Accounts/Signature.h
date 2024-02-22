//
// Created by Anh NPH on 11/09/2023.
//

#ifndef APTOS_SIGNATURE_H
#define APTOS_SIGNATURE_H

#include <vector>
#include <string>
#include "../BCS/Serialization.h"
#include "../BCS/Deserialization.h"
namespace Aptos::Accounts
{
    /// <summary>
    /// Representation of a ED25519 signature
    /// </summary>
    class Signature : public BCS::ISerializable
    {
    public:
        /// <summary>
        /// Signature length
        /// </summary>
        static const int SignatureLength = 64;

    private:
        /// <summary>
        /// Byte representation of the signature
        /// </summary>
        CryptoPP::SecByteBlock _signatureBytes;

    public:
        /// <summary>
        /// Initialize the signature.
        /// </summary>
        /// <param name="signature">The raw signature in byte array format.</param>
        explicit Signature(const CryptoPP::SecByteBlock &signature);

        /// <summary>
        /// The signature data in 64-bytes.
        /// </summary>
        /// <returns>64-byte array representing the signature data</returns>
        CryptoPP::SecByteBlock Data() const;

        /// <summary>
        /// Serialize signature
        /// </summary>
        /// <param name="serializer">Serializer object</param>
        void Serialize(BCS::Serialization &serializer) const override;
        static std::shared_ptr<BCS::ISerializable> Deserialize(BCS::Deserialization &deserializer);
        bool operator==(const Signature &other) const;

        /// <inheritdoc cref="ToString"/>
        std::string ToString() const override;

        /// <inheritdoc cref="Equals(object)"/>
        bool Equals(const Signature &rhs) const;
    };
}
#endif // APTOS_SIGNATURE_H
