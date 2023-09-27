#include <string>
#include "MultiEd25519PublicKey.h"
#include <cryptopp/xed25519.h>
namespace Aptos {
namespace Accounts {
namespace Types {

MultiEd25519PublicKey::MultiEd25519PublicKey(std::vector<unsigned char*> PublicKeys, int threshold)
{
    if (threshold > MAX_SIGNATURES_SUPPORTED)
        throw std::invalid_argument("Threshold cannot be larger than " + std::to_string(MAX_SIGNATURES_SUPPORTED));

    this->PublicKeys = PublicKeys;
    this->threshold = threshold;
}

    std::vector<unsigned char> MultiEd25519PublicKey::ToBytes()
    {
//        const size_t publicKeySize = CryptoPP::ed25519::Signer::PUBLIC_KEYLENGTH; // The size of each public key in bytes
//        std::vector<unsigned char> bytes((PublicKeys.size() * publicKeySize) + 1);
//        for (size_t i = 0; i < PublicKeys.size(); i++)
//            std::copy(PublicKeys[i].begin(), PublicKeys[i].end(), bytes.begin() + i * publicKeySize);
//
//        bytes[PublicKeys.size() * publicKeySize] = (unsigned char) threshold;
//
//        return bytes;
    }
} // namespace Types
} // namespace Accounts
} // namespace Aptos