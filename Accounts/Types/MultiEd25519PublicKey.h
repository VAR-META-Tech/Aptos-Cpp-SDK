#ifndef MULTI_ED25519_PUBLIC_KEY_H
#define MULTI_ED25519_PUBLIC_KEY_H

#include <vector>
#include <stdexcept>

namespace Aptos {
namespace Accounts {
namespace Types {

class MultiEd25519PublicKey {
public:
    static constexpr int MAX_SIGNATURES_SUPPORTED = 32;

    std::vector<unsigned char*> PublicKeys;
    int threshold;

    MultiEd25519PublicKey(std::vector<unsigned char*> PublicKeys, int threshold);

    std::vector<unsigned char> ToBytes();
};

} // namespace Types
} // namespace Accounts
} // namespace Aptos

#endif // MULTI_ED25519_PUBLIC_KEY_H