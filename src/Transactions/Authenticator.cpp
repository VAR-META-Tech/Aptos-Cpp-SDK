#include "Authenticator.h"
#include "BCS/Sequence.h"

using namespace Aptos::Accounts;
namespace Aptos::BCS
{
    Authenticator::Authenticator(const std::shared_ptr<IAuthenticator> &authenticator): m_authenticator(std::move(authenticator))
    {
        if (std::dynamic_pointer_cast<Ed25519Authenticator>(authenticator))
        {
            m_variant = Authenticator::ED25519;
        }
        else if (std::dynamic_pointer_cast<MultiEd25519Authenticator>(authenticator))
        {
            m_variant = Authenticator::MULTI_ED25519;
        }
        else if (std::dynamic_pointer_cast<MultiAgentAuthenticator>(authenticator))
        {
            m_variant = Authenticator::MULTI_AGENT;
        }
        else
        {
            throw std::invalid_argument("Invalid type");
        }
    }

    Authenticator::VariantType Authenticator::GetVariant() const
    {
        return m_variant;
    }

    std::shared_ptr<IAuthenticator> Authenticator::GetAuthenticator() const
    {
        return m_authenticator;
    }

    bool Authenticator::Verify(const CryptoPP::SecByteBlock &data)
    {
        return m_authenticator->Verify(data);
    }

    void Authenticator::Serialize(Serialization &serializer) const
    {
        serializer.SerializeU32AsUleb128(static_cast<uint32_t>(m_variant));
        m_authenticator->Serialize(serializer);
    }

    std::shared_ptr<Authenticator> Authenticator::Deserialize(Deserialization &deserializer)
    {
        int variant = deserializer.DeserializeUleb128();
        std::shared_ptr<IAuthenticator> authenticator = nullptr;

        if (variant == Authenticator::ED25519)
        {
            authenticator = std::dynamic_pointer_cast<Ed25519Authenticator>(Ed25519Authenticator::Deserialize(deserializer));
        }
        else if (variant == Authenticator::MULTI_ED25519)
        {
            authenticator = std::dynamic_pointer_cast<MultiEd25519Authenticator>(MultiEd25519Authenticator::Deserialize(deserializer));
        }
        else if (variant == Authenticator::MULTI_AGENT)
        {
            authenticator = std::dynamic_pointer_cast<MultiAgentAuthenticator>(MultiAgentAuthenticator::Deserialize(deserializer));
        }
        else
        {
            throw std::runtime_error("Invalid type: " + std::to_string(variant));
        }

        return std::make_shared<Authenticator>(std::move(authenticator));
    }

    bool Authenticator::Equals(const Authenticator &other) const
    {
        bool vr = m_variant == other.m_variant;
        bool vl = false;
        if (vr)
        {
            if (m_variant == Authenticator::ED25519)
            {
                vl = std::dynamic_pointer_cast<Ed25519Authenticator>(m_authenticator)->Equals(*(std::dynamic_pointer_cast<Ed25519Authenticator>(other.m_authenticator)));
            }
            else if (m_variant == Authenticator::MULTI_ED25519)
            {
                vl = false /*std::dynamic_pointer_cast<MultiEd25519Authenticator>(m_authenticator)->Equals(
                            *(std::dynamic_pointer_cast<MultiEd25519Authenticator>(other.m_authenticator)))*/
                    ;
            }
            else if (m_variant == Authenticator::MULTI_AGENT)
            {
                vl = std::dynamic_pointer_cast<MultiAgentAuthenticator>(m_authenticator)->Equals(*(std::dynamic_pointer_cast<MultiAgentAuthenticator>(other.m_authenticator)));
            }
            else
            {
                return false;
            }
            return vr && vl;
        }
        return false;
    }

    size_t Authenticator::GetHashCode() const
    {
        return m_authenticator->GetHashCode();
    }

    std::string Authenticator::ToString() const
    {
        return m_authenticator->ToString();
    }

    Ed25519Authenticator::Ed25519Authenticator(PublicKey publicKey, Ed25519Signature signature)
        : m_publicKey(publicKey), m_signature(signature) {}

    bool Ed25519Authenticator::Verify(const CryptoPP::SecByteBlock &data)
    {
        return m_publicKey.Verify(data, m_signature);
    }

    void Ed25519Authenticator::Serialize(Serialization &serializer) const
    {
        serializer.Serialize(m_publicKey);
        m_signature.Serialize(serializer);
    }

    std::shared_ptr<ISerializable> Ed25519Authenticator::Deserialize(Deserialization &deserializer)
    {
        auto key = std::dynamic_pointer_cast<PublicKey>(PublicKey::Deserialize(deserializer));
        auto signature = std::dynamic_pointer_cast<Ed25519Signature>(Ed25519Signature::Deserialize(deserializer));
        return std::make_shared<Ed25519Authenticator>(*key, *signature);
    }

    bool Ed25519Authenticator::Equals(const Ed25519Authenticator &other) const
    {
        return m_publicKey.Equals(other.m_publicKey) && m_signature.Equals(other.m_signature);
    }

    size_t Ed25519Authenticator::GetHashCode() const
    {
        return 0;
        //    return m_publicKey.GetHashCode() + m_signature.GetHashCode();
    }

    std::string Ed25519Authenticator::ToString() const
    {
        return "PublicKey: " + m_publicKey.ToString() + ", Ed25519Signature: " + m_signature.ToString();
    }

    MultiAgentAuthenticator::MultiAgentAuthenticator(Authenticator sender, const std::vector<std::tuple<std::shared_ptr<AccountAddress>, std::shared_ptr<Authenticator>>> &secondarySigners)
        : m_sender(sender), m_secondarySigners(secondarySigners) {}

    Sequence MultiAgentAuthenticator::SecondaryAddresses() const
    {
        std::vector<std::shared_ptr<ISerializable>> secondaryAddresses;
        for (const auto &signer : m_secondarySigners)
        {
            secondaryAddresses.push_back(std::get<0>(signer));
        }
        return Sequence(secondaryAddresses);
    }

    bool MultiAgentAuthenticator::Verify(const CryptoPP::SecByteBlock &data)
    {
        if (!m_sender.Verify(data))
        {
            return false;
        }
        for (const auto &signer : m_secondarySigners)
        {
            if (!std::get<1>(signer)->Verify(data))
            {
                return false;
            }
        }

        return true;
    }

    void MultiAgentAuthenticator::Serialize(Serialization &serializer) const
    {
        std::vector<std::shared_ptr<ISerializable>> secondaryAddresses;
        std::vector<std::shared_ptr<ISerializable>> authenticators;
        for (const auto &signer : m_secondarySigners)
        {
            secondaryAddresses.push_back(std::get<0>(signer));
            authenticators.push_back(std::get<1>(signer));
        }

        serializer.Serialize(m_sender);
        serializer.Serialize(Sequence(secondaryAddresses));
        serializer.Serialize(Sequence(authenticators));
    }

    std::shared_ptr<ISerializable> MultiAgentAuthenticator::Deserialize(Deserialization &deserializer)
    {
        std::shared_ptr<Authenticator> sender = std::dynamic_pointer_cast<Authenticator>(Authenticator::Deserialize(deserializer));
        std::vector<std::shared_ptr<ISerializable>> secondaryAddressesSeq = deserializer.DeserializeSequence(
            [](Deserialization &d) -> std::shared_ptr<ISerializable>
            { return AccountAddress::Deserialize(d); });
        std::vector<std::shared_ptr<ISerializable>> authenticatorsSeq = deserializer.DeserializeSequence(
            [](Deserialization &d) -> std::shared_ptr<ISerializable>
            { return Authenticator::Deserialize(d); });

        std::vector<std::tuple<std::shared_ptr<AccountAddress>, std::shared_ptr<Authenticator>>> secondarySigners;
        for (size_t i = 0; i < secondaryAddressesSeq.size(); i++)
        {
            // Assuming secondarySigners is a vector of tuples and the casts are valid.
            secondarySigners.emplace_back(
                std::dynamic_pointer_cast<AccountAddress>(secondaryAddressesSeq[i]),
                std::dynamic_pointer_cast<Authenticator>(authenticatorsSeq[i]));
        }

        return std::make_shared<MultiAgentAuthenticator>(*sender, secondarySigners);
    }

    bool MultiAgentAuthenticator::Equals(const MultiAgentAuthenticator &other) const
    {

        bool areEqual = m_secondarySigners.size() == other.m_secondarySigners.size();

        if (areEqual)
        {
            for (size_t i = 0; i < m_secondarySigners.size(); ++i)
            {
                std::shared_ptr<AccountAddress> addr1 = std::get<0>(m_secondarySigners[i]);
                std::shared_ptr<Authenticator> auth1 = std::get<1>(m_secondarySigners[i]);

                std::shared_ptr<AccountAddress> addr2 = std::get<0>(other.m_secondarySigners[i]);
                std::shared_ptr<Authenticator> auth2 = std::get<1>(other.m_secondarySigners[i]);

                if (!(*addr1 == *addr2) || !(*auth1 == *auth2))
                {
                    areEqual = false;
                    break;
                }
            }
        }

        return (m_sender.Equals(other.m_sender) && areEqual);
    }

    size_t MultiAgentAuthenticator::GetHashCode() const
    {
        return 0; // You need to implement this
    }

    std::string MultiAgentAuthenticator::ToString() const
    {
        return "MultiAgentAuthenticator";
    }

    MultiEd25519Authenticator::MultiEd25519Authenticator(MultiPublicKey publicKey, MultiSignature signature)
        : m_publicKey(publicKey), m_signature(signature) {}

    bool MultiEd25519Authenticator::Verify(const CryptoPP::SecByteBlock &data)
    {
        // Implement this
        return false;
    }

    void MultiEd25519Authenticator::Serialize(Serialization &serializer) const
    {
        serializer.Serialize(m_publicKey);
        serializer.Serialize(m_signature);
    }

    bool MultiEd25519Authenticator::Equals(const MultiAgentAuthenticator &other) const
    {
        return false;
        //    return m_publicKey.Equals(other.m_publicKey) && m_signature.Equals(other.m_signature);
    }

    std::shared_ptr<ISerializable> MultiEd25519Authenticator::Deserialize(Deserialization &deserializer)
    {
        // Implement this
        return nullptr;
    }

    std::string MultiEd25519Authenticator::ToString() const
    {
        return "MultiEd25519Authenticator";
    }

    bool operator==(const Authenticator &lhs, const Authenticator &rhs)
    {
        return lhs.Equals(rhs);
    }
}
