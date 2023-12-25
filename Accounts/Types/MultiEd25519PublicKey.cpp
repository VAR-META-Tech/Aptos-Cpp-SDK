#include <string>
#include "MultiEd25519PublicKey.h"
#include <cryptopp/xed25519.h>
#include "../../HDWallet/Utils/Utils.h"

namespace Aptos::Accounts::Types
{
    MultiEd25519PublicKey::MultiEd25519PublicKey(std::vector<PublicKey> &PublicKeys, int threshold)
    {
        if (threshold > MAX_SIGNATURES_SUPPORTED)
            throw std::invalid_argument("Threshold cannot be larger than " + std::to_string(MAX_SIGNATURES_SUPPORTED));

        this->PublicKeys = PublicKeys;
        this->threshold = threshold;
    }

    CryptoPP::SecByteBlock MultiEd25519PublicKey::ToBytes()
    {
        size_t secBlockSize = (PublicKeys.size() * Utils::Ed25519PublicKeySizeInBytes) + 1;
        CryptoPP::SecByteBlock bytes(secBlockSize);
        //    for (int i = 0; i < PublicKeys.size(); i++) {
        //        bytes[i * Utils::Ed25519PublicKeySizeInBytes] = PublicKeys[i] ;
        //    }
        //    bytes[PublicKeys.size() * Utils::Ed25519PublicKeySizeInBytes] = static_cast<CryptoPP::byte>(threshold);

        return bytes;
    }
} // namespace Aptos
