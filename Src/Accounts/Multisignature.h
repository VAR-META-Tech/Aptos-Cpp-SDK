#ifndef MULTISIGNATURE_H
#define MULTISIGNATURE_H
#include <vector>
#include <tuple>
#include "Ed25519Signature.h"
#include "Ed25519/PublicKey.h"
#include "Multipublickey.h"
#include "../BCS/BCS.h"
using namespace Aptos::Accounts::Ed25519;

namespace Aptos::Accounts
{
    /// <summary>
    /// The ED25519 Multi-Signature Implementation.
    /// </summary>
    class APTOS_API MultiSignature : public BCS::ISerializable
    {
    public:
        /// <summary>
        /// Initialize the MultiSignature object.
        /// </summary>
        /// <param name="PublicKeyMulti">The multi public key object containing the public keys used to generate the Bitmap.</param>
        /// <param name="SignatureMap">The tuple list containing the public keys associated with their signatures.</param>
        MultiSignature(const MultiPublicKey &PublicKeyMulti, const std::vector<std::pair<PublicKey, Ed25519Signature>> &SignatureMap);

        /// <summary>
        /// Serialize the concatenated signatures and bitmap of an ED25519 Multi-signature instance to a Data object.
        ///
        /// This function concatenates the signatures of the instance and serializes the concatenated signatures and bitmap to a Data object.
        /// </summary>
        /// <returns>A byte list containing the serialized concatenated signatures and bitmap.</returns>
        std::vector<uint8_t> ToBytes() const;

        void Serialize(BCS::Serialization &serializer) const override;
        std::string ToString() const override;

    private:
        /// <summary>
        /// The signatures themselves.
        /// </summary>
        std::vector<Ed25519Signature> Signatures;

        /// <summary>
        /// The compact representation of which keys among a set of N possible keys have signed a given message.
        /// </summary>
        std::vector<uint8_t> Bitmap;
    };
}
#endif // MULTISIGNATURE_H
