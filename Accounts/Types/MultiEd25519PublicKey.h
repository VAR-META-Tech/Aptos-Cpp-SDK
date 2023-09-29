#ifndef MULTI_ED25519_PUBLIC_KEY_H
#define MULTI_ED25519_PUBLIC_KEY_H

#include <vector>
#include <stdexcept>
#include <cryptopp/xed25519.h>
#include "../PublicKey.h"

namespace Aptos {
namespace Accounts {
namespace Types {

class MultiEd25519PublicKey {
public:
    static constexpr int MAX_SIGNATURES_SUPPORTED = 32;

    std::vector<PublicKey> PublicKeys;
    int threshold;

    MultiEd25519PublicKey(std::vector<PublicKey> PublicKeys, int threshold);

    std::vector<CryptoPP::byte> ToBytes();
};

} // namespace Types
} // namespace Accounts
} // namespace Aptos

#endif // MULTI_ED25519_PUBLIC_KEY_H