#include "SignedTransaction.h"
#include "MultiAgentRawTransaction.h"
#include "../HDWallet/Utils/Utils.h"

namespace Aptos::BCS
{
    SignedTransaction::SignedTransaction(const RawTransaction &transaction, const Authenticator &authenticator)
        : transaction(transaction), authenticator(authenticator) {}

    std::vector<uint8_t> SignedTransaction::Bytes() const
    {
        Serialization ser;
        ser.Serialize(*this);
        return ser.GetBytes();
    }

    bool SignedTransaction::Verify()
    {
        std::vector<uint8_t> keyed;

        if (std::dynamic_pointer_cast<MultiAgentAuthenticator>(authenticator.GetAuthenticator()) != nullptr)
        {
            const std::shared_ptr<MultiAgentAuthenticator> multiAgentAuthenticator = std::dynamic_pointer_cast<MultiAgentAuthenticator>(authenticator.GetAuthenticator());

            MultiAgentRawTransaction multiAgentTransaction(this->transaction, multiAgentAuthenticator->SecondaryAddresses());

            keyed = multiAgentTransaction.Keyed();
        }
        else
        {
            keyed = this->transaction.Keyed();
        }

        return authenticator.Verify(Utils::ByteVectorToSecBlock(keyed));
    }

    void SignedTransaction::Serialize(Serialization &serializer) const
    {
        transaction.Serialize(serializer);
        authenticator.Serialize(serializer);
    }

    std::shared_ptr<SignedTransaction> SignedTransaction::Deserialize(Deserialization &deserializer)
    {
        auto transaction = std::dynamic_pointer_cast<RawTransaction>(RawTransaction::Deserialize(deserializer));
        auto authenticator = std::dynamic_pointer_cast<Authenticator>(Authenticator::Deserialize(deserializer));
        return std::make_shared<SignedTransaction>(*transaction, *authenticator);
    }

    bool SignedTransaction::Equals(const SignedTransaction &other) const
    {
        return transaction.Equals(other.transaction) && authenticator.Equals(other.authenticator);
    }

    std::string SignedTransaction::ToString() const
    {
        return "Transaction: " + transaction.ToString() + "\n Authenticator: " + authenticator.ToString();
    }

    size_t SignedTransaction::GetHashCode() const 
    {
        size_t hash = 17;
        hash = hash * 23 + transaction.GetHashCode();
        hash = hash * 23 + authenticator.GetHashCode();
        return hash;
    }

    RawTransaction SignedTransaction::getTransaction() const
    {
        return transaction;
    }

    bool operator==(const SignedTransaction &lhs, const SignedTransaction &rhs)
    {
        return lhs.Equals(rhs);
    }
}