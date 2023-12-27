#include "multisignature.h"
#include "../HDWallet/Utils/Utils.h"
#if defined(__APPLE__)
#include <libkern/OSByteOrder.h>

#define htobe32(x) OSSwapHostToBigInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#endif

namespace Aptos::Accounts
{
    MultiSignature::MultiSignature(const MultiPublicKey &PublicKeyMulti, const std::vector<std::pair<PublicKey, Signature>> &SignatureMap)
    {
        int bitmap = 0;
        auto keys = PublicKeyMulti.getKeys();
        for (const auto &[key, value] : SignatureMap)
        {
            this->Signatures.push_back(value);
            int index = std::distance(keys.begin(), std::find(keys.begin(), keys.end(), key));
            int shift = 31 - index;
            bitmap = bitmap | (1 << shift);
        }

        uint32_t uBitmap = htobe32(static_cast<uint32_t>(bitmap));
        this->Bitmap = std::vector<uint8_t>(std::bit_cast<uint8_t *>(&uBitmap), std::bit_cast<uint8_t *>(&uBitmap) + sizeof(uint32_t));
    }

    std::vector<uint8_t> MultiSignature::ToBytes() const
    {
        std::vector<uint8_t> concatenatedSignatures;
        for (const Signature &signature : Signatures)
        {
            std::vector<uint8_t> signatureBytes = Utils::SecBlockToByteVector(signature.Data());
            concatenatedSignatures.insert(std::end(concatenatedSignatures), std::begin(signatureBytes), std::end(signatureBytes));
        }
        concatenatedSignatures.insert(std::end(concatenatedSignatures), std::begin(Bitmap), std::end(Bitmap));
        return concatenatedSignatures;
    }

    void MultiSignature::Serialize(BCS::Serialization &serializer) const
    {
        std::vector<uint8_t> bytes = ToBytes();
        serializer.SerializeBytes(bytes);
    }

    std::string MultiSignature::ToString() const
    {
        return "MultiSignature";
    }
}