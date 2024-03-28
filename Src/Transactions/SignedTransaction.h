#ifndef SIGNEDTRANSACTION_H
#define SIGNEDTRANSACTION_H

#include <iostream>
#include "BCS/BCS.h"
#include "Rawtransaction.h"
#include "Authenticator.h"

namespace Aptos::BCS
{
    /// <summary>
    /// Signed transaction implementation.
    /// </summary>
    class APTOS_API SignedTransaction : public BCS::ISerializable
    {
    public:
        explicit SignedTransaction(const RawTransaction &transaction, const Authenticator &authenticator);
        std::vector<uint8_t> Bytes() const;
        bool Verify();
        void Serialize(Serialization &serializer) const override;
        static std::shared_ptr<SignedTransaction> Deserialize(Deserialization &deserializer);
        bool Equals(const SignedTransaction &other) const;
        std::string ToString() const override;
        size_t GetHashCode() const override;
        RawTransaction getTransaction() const;

    private:
        RawTransaction transaction;
        Authenticator authenticator;
    };

    bool APTOS_API operator==(const SignedTransaction &lhs, const SignedTransaction &rhs);
}
#endif // SIGNEDTRANSACTION_H
