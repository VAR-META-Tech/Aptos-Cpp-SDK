#ifndef MULTIPUBLICKEY_H
#define MULTIPUBLICKEY_H

#include <vector>
#include <stdexcept>
#include "Ed25519/PublicKey.h"
#include "../BCS/BCS.h"
using namespace Aptos::Accounts::Ed25519;

namespace Aptos::Accounts
{
    /// <summary>
    /// The ED25519 Multi-Public Key implementation.
    /// </summary>
    class APTOS_API MultiPublicKey : public BCS::ISerializable
    {
    public:
        /// <summary>
        /// The minimum amount of keys to initialize this class.
        /// </summary>
        static const int MIN_KEYS = 2;

        /// <summary>
        /// The maximum amount of keys allowed for initialization.
        /// </summary>
        static const int MAX_KEYS = 32;

        /// <summary>
        /// The minimum threshold amount.
        /// </summary>
        static const int MIN_THRESHOLD = 1;

        /// <summary>
        /// Initializer for the MultiPublicKey.
        /// </summary>
        /// <param name="keys">The Public Keys themselves.</param>
        /// <param name="threshold">The current amount of keys in the keys array.</param>
        /// <param name="checked">Verify whether the amount of keys fit within the threshold from 2 to 32 keys, both sides are inclusive.</param>
        /// <exception cref="Exception"></exception>
        MultiPublicKey(const std::vector<PublicKey> &keys, CryptoPP::byte threshold, bool checked = true);

        /// <summary>
        /// Serialize the threshold and concatenated keys of a given threshold signature scheme instance to a Data object.
        ///
        /// This function concatenates the keys of the instance and serializes the threshold and concatenated keys to a Data object.
        /// </summary>
        /// <returns>A bytes array containing the serialized threshold and concatenated keys.</returns>
        std::vector<uint8_t> ToBytes() const;

        /// <summary>
        /// Deserialize a Data object to a MultiPublicKey instance.
        ///
        /// This function deserializes the given Data object to a MultiPublicKey instance by extracting the threshold and keys from it.
        /// </summary>
        /// <param name="keyBytes">A Data object containing the serialized threshold and keys of a MultiPublicKey instance.</param>
        /// <returns>A MultiPublicKey instance initialized with the deserialized keys and threshold.</returns>
        /// <exception cref="Exception"></exception>
        static MultiPublicKey FromBytes(const std::vector<uint8_t> &keyBytes);

        void Serialize(BCS::Serialization &serializer) const;
        std::string ToString() const;

        std::vector<PublicKey> getKeys() const;

    private:
        /// <summary>
        /// The Public Keys themselves.
        /// </summary>
        std::vector<PublicKey> Keys;

        /// <summary>
        /// The current amount of keys in the keys array.
        /// </summary>
        CryptoPP::byte Threshold;
    };
}
#endif // MULTIPUBLICKEY_H
