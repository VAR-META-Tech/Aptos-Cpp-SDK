#ifndef MULTIAGENTRAWTRANSACTION_H
#define MULTIAGENTRAWTRANSACTION_H

#include <string>
#include <vector>
#include "rawtransaction.h"
#include "BCS/Sequence.h"
namespace Aptos::BCS
{
    class MultiAgentRawTransaction
    {
    public:
        explicit MultiAgentRawTransaction(const RawTransaction &rawTransaction, const Sequence &secondarySigners);

        RawTransaction Inner();
        std::vector<uint8_t> Prehash();
        std::vector<uint8_t> Keyed();
        Accounts::Signature Sign(Accounts::PrivateKey key);
        bool Verify(const Accounts::PublicKey &key, const Accounts::Signature &signature);

    private:
        RawTransaction rawTransaction;
        Sequence secondarySigners;
    };
}
#endif // MULTIAGENTRAWTRANSACTION_H
