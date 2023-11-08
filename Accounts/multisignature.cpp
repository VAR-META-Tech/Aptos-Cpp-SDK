#include "multisignature.h"
#include "../HDWallet/Utils/Utils.h"

MultiSignature::MultiSignature(MultiPublicKey PublicKeyMulti, std::vector<std::tuple<PublicKey, Signature> > SignatureMap)
{
    Signatures = std::vector<Signature>();
//    uint32_t bitmap = 0;
//    for (std::tuple<PublicKey, Signature> entry : SignatureMap)
//    {
//        Signatures.push_back(std::get<1>(entry));
//        int index = PublicKeyMulti.Keys.IndexOf(std::get<0>(entry));
//        int shift = 31 - index; // 32 bit positions, left to right.
//        bitmap = bitmap | (1 << shift);
//    }

//    // 4-byte big endian bitmap.
//    // this.Bitmap = bitmap.to_bytes(4, "big")
//    uint32_t uBitmap = htonl(bitmap);
//    Bitmap = std::vector<uint8_t>((uint8_t*)&uBitmap, (uint8_t*)&uBitmap + sizeof(uBitmap));
}

std::vector<uint8_t> MultiSignature::ToBytes() const
{
    std::vector<uint8_t> concatenatedSignatures;
    for (Signature signature : Signatures)
    {
        std::vector<uint8_t> signatureBytes = Utils::SecBlockToByteVector(signature.Data());
        concatenatedSignatures.insert(std::end(concatenatedSignatures), std::begin(signatureBytes), std::end(signatureBytes));
    }
    concatenatedSignatures.insert(std::end(concatenatedSignatures), std::begin(Bitmap), std::end(Bitmap));
    return concatenatedSignatures;
}

void MultiSignature::Serialize(Serialization& serializer) const
{
    std::vector<uint8_t> bytes = ToBytes();
    serializer.SerializeBytes(bytes);
}

std::string MultiSignature::ToString() const
{
    return "MultiSignature";
}
