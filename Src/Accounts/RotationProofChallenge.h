#ifndef ROTATIONPROOFCHALLENGE_H
#define ROTATIONPROOFCHALLENGE_H
#include <string>
#include "Account.h"
#include "AccountAddress.h"

namespace Aptos::Accounts
{

class APTOS_API RotationProofChallenge
{
private:
    AccountAddress TypeInfoAccountAddress = AccountAddress::FromHex("0x1");
    std::string TypeInfoModuleName = "account";
    std::string TypeInfoStructName = "RotationProofChallenge";
    int SequenceNumber;
    AccountAddress Originator;
    AccountAddress CurrentAuthKey;
    std::vector<uint8_t> NewPublicKey;

public:
    RotationProofChallenge(
        int sequenceNumber,
        const AccountAddress& originator,
        const AccountAddress& currentAuthKey,
        const std::vector<uint8_t>& newPublicKey
        );

    void Serialize(BCS::Serialization& serializer);
};
}
#endif // ROTATIONPROOFCHALLENGE_H
