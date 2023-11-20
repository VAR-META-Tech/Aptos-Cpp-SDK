#ifndef MULTIAGENTRAWTRANSACTION_H
#define MULTIAGENTRAWTRANSACTION_H

#include <string>
#include <vector>
#include "rawtransaction.h"
#include "Sequence.h"

using namespace Aptos::Accounts;
namespace Aptos::BCS
{
    class MultiAgentRawTransaction
    {
    public:
        MultiAgentRawTransaction(const RawTransaction &rawTransaction, const Sequence &secondarySigners);

        RawTransaction Inner();
        std::vector<uint8_t> Prehash();
        std::vector<uint8_t> Keyed();
        Signature Sign(PrivateKey key);
        bool Verify(const PublicKey &key, const Signature &signature);

    private:
        RawTransaction rawTransaction;
        Sequence secondarySigners;
    };
}
#endif // MULTIAGENTRAWTRANSACTION_H
