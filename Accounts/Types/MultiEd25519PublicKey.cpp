#include <string>
#include "MultiEd25519PublicKey.h"
#include <cryptopp/xed25519.h>
namespace Aptos {
namespace Accounts {
namespace Types {
    MultiEd25519PublicKey::MultiEd25519PublicKey(std::vector<PublicKey> PublicKeys, int threshold)
    {
        if (threshold > MAX_SIGNATURES_SUPPORTED)
            throw std::invalid_argument("Threshold cannot be larger than " + std::to_string(MAX_SIGNATURES_SUPPORTED));

        this->PublicKeys = PublicKeys;
        this->threshold = threshold;
    }

    std::vector<unsigned char> MultiEd25519PublicKey::ToBytes()
    {
        std::vector<CryptoPP::byte> bytes;
        bytes.resize((PublicKeys.size() * CryptoPP::ed25519PublicKey::PUBLIC_KEYLENGTH) + 1);
        for (int i = 0; i < PublicKeys.size(); i++)
            bytes.insert(bytes.end(),
                         PublicKeys[i].KeyBytes().begin(),PublicKeys[i].KeyBytes().end());

        bytes[PublicKeys.size() * CryptoPP::ed25519PublicKey::PUBLIC_KEYLENGTH] = threshold;

        return bytes;
    }
} // namespace Types
} // namespace Accounts
} // namespace Aptos