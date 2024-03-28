#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include "BCS/BCS.h"
#include "../Accounts/Ed25519Signature.h"
#include "../Accounts/Ed25519/PublicKey.h"
#include "../Accounts/AccountAddress.h"
#include "../Accounts/Multisignature.h"

using namespace Aptos::Accounts::Ed25519;

namespace Aptos::BCS
{
    /// <summary>
    /// Each transaction submitted to the Aptos blockchain contains a `TransactionAuthenticator`.
    /// During transaction execution, the executor will check if every `AccountAuthenticator`'s
    /// signature on the transaction hash is well-formed and whether the sha3 hash of the
    /// `AccountAuthenticator`'s `AuthenticationKeyPreimage` matches the `AuthenticationKey` stored
    /// under the participating signer's account address.
    /// </summary>
    class APTOS_API IAuthenticator : public ISerializable
    {
    public:
        virtual bool Verify(const CryptoPP::SecByteBlock &data) = 0;
    };

    /// <summary>
    /// A generic Authenticator.
    /// </summary>
    class APTOS_API Authenticator : public IAuthenticator
    {
    public:
        enum VariantType
        {
            ED25519 = 0,
            MULTI_ED25519 = 1,
            MULTI_AGENT = 2
        };

        /// <summary>
        /// Creates an Authenticator from a given concrete authenticator.
        /// </summary>
        /// <param name="authenticator">A concrete authenticator.</param>
        explicit Authenticator(const std::shared_ptr<IAuthenticator> &authenticator);

        /// <summary>
        /// Returns that type of Authenticator.
        /// </summary>
        /// <returns>An integer that represents the type of Authenticator.</returns>
        VariantType GetVariant() const;

        /// <summary>
        /// Returns the Authenticator.
        /// </summary>
        /// <returns>An Authenticator object.</returns>
        std::shared_ptr<IAuthenticator> GetAuthenticator() const;

        /// <summary>
        /// Verifies the signed data.
        /// </summary>
        /// <param name="data"></param>
        /// <returns>True is signature can be verified, false otherwise.</returns>
        bool Verify(const CryptoPP::SecByteBlock &data) override;

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static std::shared_ptr<Authenticator> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        bool Equals(const Authenticator &other) const;

        /// <inheritdoc/>
        size_t GetHashCode() const override;

        /// <inheritdoc/>
        std::string ToString() const override;

    private:
        VariantType m_variant;
        std::shared_ptr<IAuthenticator> m_authenticator;
    };

    bool APTOS_API operator==(const Authenticator &lhs, const Authenticator &rhs);

    /// <summary>
    /// ED25519 Authenticator.
    /// </summary>
    class APTOS_API Ed25519Authenticator : public IAuthenticator
    {
    public:
        /// <summary>
        /// Creates an Ed25519Authenticator using a given public key and signature.
        /// </summary>
        /// <param name="publicKey"></param>
        /// <param name="signature"></param>
        Ed25519Authenticator(PublicKey publicKey, Accounts::Ed25519Signature signature);

        /// <summary>
        /// Verifies the data with the signature.
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        bool Verify(const CryptoPP::SecByteBlock &data) override;

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static std::shared_ptr<ISerializable> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        bool Equals(const Ed25519Authenticator &other) const;

        /// <inheritdoc/>
        size_t GetHashCode() const override;

        /// <inheritdoc/>
        std::string ToString() const override;

    protected:
        /// <summary>
        /// The authenticators public key.
        /// </summary>
        PublicKey m_publicKey;

        /// <summary>
        /// The authenticator's public key.
        /// </summary>
        Accounts::Ed25519Signature m_signature;
    };

    /// <summary>
    /// An Authenticator that uses a list of tuples of account addresses and authenticator pairs.
    /// </summary>
    class APTOS_API MultiAgentAuthenticator : public IAuthenticator
    {
    public:
        /// <summary>
        /// Creates a MultiAgentAuthenticator object from a given sender
        /// authenticator, and a list of acount address to authenticator tuples.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="secondarySigners"></param>
        MultiAgentAuthenticator(Authenticator sender,
                                const std::vector<std::tuple<std::shared_ptr<Accounts::AccountAddress>, std::shared_ptr<Authenticator>>> &secondarySigners);

        /// <summary>
        /// Returns the list (Sequence) of corresponding account addresses.
        /// </summary>
        /// <returns>A Sequence of account addresses</returns>
        Sequence SecondaryAddresses() const;

        /// <summary>
        /// Verifies the data with all the account addresses.
        /// </summary>
        /// <param name="data"></param>
        /// <returns>True if all accouncts can verify, false otherwise.</returns>
        bool Verify(const CryptoPP::SecByteBlock &data) override;

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static std::shared_ptr<ISerializable> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        bool Equals(const MultiAgentAuthenticator &other) const;

        /// <inheritdoc/>
        size_t GetHashCode() const override;

        /// <inheritdoc/>
        std::string ToString() const override;

    private:
        /// <summary>
        /// The multi agent authenticator's sender's authenticator.
        /// </summary>
        Authenticator m_sender;

        /// <summary>
        /// A list of acount address to authenticator tuples.
        /// </summary>
        std::vector<std::tuple<std::shared_ptr<Accounts::AccountAddress>, std::shared_ptr<Authenticator>>> m_secondarySigners;
    };

    /// <summary>
    /// MultiEd25519Authenticator
    /// </summary>
    class APTOS_API MultiEd25519Authenticator : public IAuthenticator
    {
    public:
        /// <summary>
        /// Creates a MultiEd25519Authenticator from a given multi-public key
        /// and multi-signature.
        /// </summary>
        /// <param name="publicKey"></param>
        /// <param name="signature"></param>
        MultiEd25519Authenticator(Accounts::MultiPublicKey publicKey, Accounts::MultiSignature signature);

        /// <inheritdoc/>
        bool Verify(const CryptoPP::SecByteBlock &data) override;

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        bool Equals(const MultiAgentAuthenticator &other) const;

        /// <inheritdoc/>
        static std::shared_ptr<ISerializable> Deserialize(Deserialization &deserializer);
        std::string ToString() const override;

    private:
        /// <summary>
        /// The authenticator's multi-public key.
        /// </summary>
        Accounts::MultiPublicKey m_publicKey;

        /// <summary>
        /// The authenticator's multi-signature.
        /// </summary>
        Accounts::MultiSignature m_signature;
    };
}
#endif // AUTHENTICATOR_H
