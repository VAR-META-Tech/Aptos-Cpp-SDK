#include "RotationProofChallenge.h"

Aptos::Accounts::RotationProofChallenge::RotationProofChallenge(int sequenceNumber, const AccountAddress &originator, const AccountAddress &currentAuthKey, const std::vector<uint8_t> &newPublicKey)
    : SequenceNumber(sequenceNumber), Originator(originator), CurrentAuthKey(currentAuthKey), NewPublicKey(newPublicKey)
{
}

void Aptos::Accounts::RotationProofChallenge::Serialize(BCS::Serialization &serializer)
{
    TypeInfoAccountAddress.Serialize(serializer);
    serializer.SerializeString(TypeInfoModuleName);
    serializer.SerializeString(TypeInfoStructName);
    serializer.SerializeU64(static_cast<uint64_t>(SequenceNumber));
    Originator.Serialize(serializer);
    CurrentAuthKey.Serialize(serializer);
    serializer.SerializeBytes(NewPublicKey);
}
