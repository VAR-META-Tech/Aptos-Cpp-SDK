#ifndef MULTI_ED25519_PUBLIC_KEY_H
#define MULTI_ED25519_PUBLIC_KEY_H

#include <vector>
#include <stdexcept>
#include <cryptopp/xed25519.h>
#include "../Ed25519/PublicKey.h"
#include <sstream>

using namespace Aptos::Accounts::Ed25519;
namespace Aptos
{
    namespace Accounts
    {
        namespace Types
        {

            /// <summary>
            /// Represents a multi-signature public key.
            /// More details can be found <see cref="https://aptos.dev/guides/creating-a-signed-transaction/#multisignature-transactions">here</see>.
            /// </summary>
            class APTOS_API MultiEd25519PublicKey
            {
            public:
                /// Max number of signature supported
                static constexpr int MAX_SIGNATURES_SUPPORTED = 32;

                /// List of public keys
                std::vector<PublicKey> PublicKeys;

                /// The max number of keys accepted in the multi-sig.
                int threshold;

                /// <summary>
                /// Public key for a K-of-N multisig transaction. A K-of-N multisig transaction means that for such a \n
                /// transaction to be executed, at least K out of the N authorized signers have signed the transaction \n
                /// and passed the check conducted by the chain. \n
                ///  See <see cref="https://aptos.dev/guides/creating-a-signed-transaction#multisignature-transactions">Creating a Signed Transaction</see>.
                /// </summary>
                /// <param name="PublicKeys">List of public keys used to create the multi-sig</param>
                /// <param name="threshold">The max number of keys accepted in the multi-sig.</param>
                MultiEd25519PublicKey(std::vector<PublicKey> &PublicKeys, int threshold);

                /// <summary>
                /// Converts a MultiEd25519PublicKey into bytes \n
                /// with: <code>bytes = p1_bytes | ... | pn_bytes | threshold</code>.
                /// </summary>
                /// <returns>Byte array representing multi-sign public key.</returns>
                CryptoPP::SecByteBlock ToBytes();
            };

        } // namespace Types
    }     // namespace Accounts
} // namespace Aptos

#endif // MULTI_ED25519_PUBLIC_KEY_H
