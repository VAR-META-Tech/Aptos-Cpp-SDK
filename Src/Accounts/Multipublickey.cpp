#include "Multipublickey.h"
#include "../HDWallet/Utils/Utils.h"

namespace Aptos::Accounts
{
    MultiPublicKey::MultiPublicKey(const std::vector<PublicKey> &keys, CryptoPP::byte threshold, bool checked)
    {
        if (checked)
        {
            if (!(MIN_KEYS <= keys.size() && keys.size() <= MAX_KEYS))
                throw std::invalid_argument("Must have between " + std::to_string(MIN_KEYS) + " and " + std::to_string(MAX_KEYS) + " keys.");

            if (!(MIN_THRESHOLD <= threshold && threshold < keys.size()))
                throw std::invalid_argument("Threshold must be between " + std::to_string(MIN_THRESHOLD) + " and " + std::to_string(keys.size()));
        }

        Keys = keys;
        Threshold = threshold;
    }

    std::vector<uint8_t> MultiPublicKey::ToBytes() const
    {
        CryptoPP::SecByteBlock concatenatedKeys;
        for (const PublicKey &key : Keys)
        {
            concatenatedKeys.Append(key.KeyBytes());
        }
        concatenatedKeys.Append(1, Threshold);
        return Utils::SecBlockToByteVector(concatenatedKeys);
    }

    MultiPublicKey MultiPublicKey::FromBytes(const std::vector<uint8_t> &keyBytes)
    {
        const int minKeys = MIN_KEYS;
        const int maxKeys = MAX_KEYS;
        const int minThreshold = MIN_THRESHOLD;

        int nSigners = keyBytes.size() / Utils::Ed25519PublicKeySizeInBytes;
        if (!(minKeys <= nSigners && nSigners <= maxKeys))
        {
            throw std::invalid_argument("Must have between " + std::to_string(minKeys) + " and " + std::to_string(maxKeys) + " keys.");
        }
        CryptoPP::byte threshold = keyBytes[keyBytes.size() - 1];
        if (!(minThreshold <= threshold && threshold <= nSigners))
        {
            throw std::invalid_argument("Threshold must be between " + std::to_string(minThreshold) + " and " + std::to_string(nSigners));
        }
        std::vector<PublicKey> keys;
        for (int i = 0; i < nSigners; i++)
        {
            int startByte = i * Utils::Ed25519PublicKeySizeInBytes;
            int endByte = (i + 1) * Utils::Ed25519PublicKeySizeInBytes;
            std::vector<uint8_t> tempKey(keyBytes.begin() + startByte, keyBytes.begin() + endByte);
            PublicKey publicKey(Utils::ByteVectorToSecBlock(tempKey));
            keys.push_back(publicKey);
        }

        return MultiPublicKey(keys, threshold);
    }

    void MultiPublicKey::Serialize(BCS::Serialization &serializer) const
    {
        auto bytes = this->ToBytes();
        serializer.SerializeBytes(bytes);
    }

    std::string MultiPublicKey::ToString() const
    {
        return std::to_string(this->Threshold) + "-of-" + std::to_string(this->Keys.size()) + " Multi-Ed25519 public key";
    }

    std::vector<PublicKey> MultiPublicKey::getKeys() const
    {
        return Keys;
    }
}