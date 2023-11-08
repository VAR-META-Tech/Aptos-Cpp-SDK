#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include "BCSTypes.h"
#include "Serialization.h"
#include "../Accounts/Signature.h"
#include "../Accounts/PublicKey.h"
#include "Sequence.h"
#include "../Accounts/AccountAddress.h"
#include "../Accounts/multisignature.h"

class IAuthenticator : public ISerializable {
public:
    virtual bool Verify(const CryptoPP::SecByteBlock& data) = 0;
};

class Authenticator : public IAuthenticator {
public:
    enum VariantType {
        ED25519 = 0,
        MULTI_ED25519 = 1,
        MULTI_AGENT = 2
    };

    Authenticator(const std::shared_ptr<IAuthenticator>& authenticator);
    VariantType GetVariant() const;
    std::shared_ptr<IAuthenticator> GetAuthenticator() const;
    bool Verify(const CryptoPP::SecByteBlock& data) override;
    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    bool Equals(const Authenticator& other) const;
    int GetHashCode() const;
    std::string ToString() const override;

private:
    VariantType Variant;
    std::shared_ptr<IAuthenticator> m_authenticator;
};

bool operator==(const Authenticator &lhs, const Authenticator &rhs);

class Ed25519Authenticator : public IAuthenticator {
public:
    Ed25519Authenticator(PublicKey publicKey, Signature signature);
    bool Verify(const CryptoPP::SecByteBlock &data) override;
    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    bool Equals(const Ed25519Authenticator& other) const;
    int GetHashCode() const;
    std::string ToString() const override;
protected:
    PublicKey m_publicKey;
    Signature m_signature;
};

class MultiAgentAuthenticator : public IAuthenticator {
public:
    MultiAgentAuthenticator(Authenticator sender,
                            const std::vector<std::tuple<std::shared_ptr<AccountAddress>, std::shared_ptr<Authenticator>>>& secondarySigners);
    Sequence SecondaryAddresses() const;
    bool Verify(const CryptoPP::SecByteBlock& data) override;
    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    bool Equals(const MultiAgentAuthenticator& other) const;
    int GetHashCode() const;
    std::string ToString() const override;

private:
    Authenticator sender;
    std::vector<std::tuple<std::shared_ptr<AccountAddress>, std::shared_ptr<Authenticator>>> secondarySigners;
};

class MultiEd25519Authenticator : public IAuthenticator {
public:
    MultiEd25519Authenticator(MultiPublicKey publicKey, MultiSignature signature);
    bool Verify(const CryptoPP::SecByteBlock& data) override;
    void Serialize(Serialization& serializer) const override;
    bool Equals(const MultiAgentAuthenticator& other) const;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
private:
    MultiPublicKey m_publicKey;
    MultiSignature m_signature;
};

#endif // AUTHENTICATOR_H
