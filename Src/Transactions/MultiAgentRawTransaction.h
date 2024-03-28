#ifndef MULTIAGENTRAWTRANSACTION_H
#define MULTIAGENTRAWTRANSACTION_H

#include <string>
#include <vector>
#include "Rawtransaction.h"
#include "BCS/BCS.h"

using namespace Aptos::Accounts::Ed25519;

namespace Aptos::BCS
{
    class APTOS_API MultiAgentRawTransaction
    {
    public:
        explicit MultiAgentRawTransaction(const RawTransaction &rawTransaction, const Sequence &secondarySigners);

        RawTransaction Inner();
        std::vector<uint8_t> Prehash();
        std::vector<uint8_t> Keyed();
        Accounts::Ed25519Signature Sign(PrivateKey key);
        bool Verify(const PublicKey &key, const Accounts::Ed25519Signature &signature);

    private:
        RawTransaction rawTransaction;
        Sequence secondarySigners;
    };
}
#endif // MULTIAGENTRAWTRANSACTION_H
