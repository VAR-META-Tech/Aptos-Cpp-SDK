#ifndef SIGNEDTRANSACTION_H
#define SIGNEDTRANSACTION_H


#include <iostream>
#include "BCSTypes.h"
#include "rawtransaction.h"
#include "Authenticator.h"

class SignedTransaction : public ISerializable {
public:
    SignedTransaction(const RawTransaction& transaction, const Authenticator& authenticator);
    std::vector<uint8_t> Bytes();
    bool Verify();
    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    bool Equals(const SignedTransaction& other) const;
    std::string ToString() const override;
    size_t GetHashCode() override;
    RawTransaction getTransaction() const;

private:
    RawTransaction transaction;
    Authenticator authenticator;
};


bool operator==(const SignedTransaction &lhs, const SignedTransaction &rhs);

#endif // SIGNEDTRANSACTION_H
