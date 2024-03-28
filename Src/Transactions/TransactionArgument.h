#ifndef TRANSACTIONARGUMENT_H
#define TRANSACTIONARGUMENT_H
#include <iostream>
#include <vector>
#include "BCS/BCS.h"

namespace Aptos::BCS
{
    class APTOS_API TransactionArgument
    {
    public:
        explicit TransactionArgument(ISerializable *value, TypeTag encoderType);
        std::vector<uint8_t> Encode();

    private:
        ISerializable *value;
        TypeTag encoderType;
    };
}
#endif // TRANSACTIONARGUMENT_H
