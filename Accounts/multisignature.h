#ifndef MULTISIGNATURE_H
#define MULTISIGNATURE_H



#include <vector>
#include <tuple>
#include "Signature.h"
#include "PublicKey.h"
#include "multipublickey.h"
#include "../BCS/BCSTypes.h"

class MultiSignature : public ISerializable
{
public:
    MultiSignature(MultiPublicKey PublicKeyMulti,
                   std::vector<std::tuple<PublicKey, Signature>> SignatureMap);

    std::vector<uint8_t> ToBytes() const;

    void Serialize(Serialization& serializer) const override;
    std::string ToString() const override;

private:
    std::vector<Signature> Signatures;
    std::vector<uint8_t> Bitmap;
};
#endif // MULTISIGNATURE_H
