#ifndef TRANSACTIONARGUMENT_H
#define TRANSACTIONARGUMENT_H

#include <iostream>
#include <vector>
#include "BCSTypes.h"

class TransactionArgument {
public:
    TransactionArgument(ISerializable* value, TypeTag encoderType);
    std::vector<uint8_t> Encode();
private:
    ISerializable* value;
    TypeTag encoderType;
};

#endif // TRANSACTIONARGUMENT_H
