#ifndef TRANSACTIONCONVERTER_H
#define TRANSACTIONCONVERTER_H

#include "Transaction.h"

namespace AptosRESTModel {

class APTOS_API TransactionConverter
{
public:
    TransactionConverter();
    static Transaction ProcessTransactionContents(const nlohmann::json& item, const std::string& type = "");
    static Transaction ReadJson(const nlohmann::json& item);
};

}

#endif // TRANSACTIONCONVERTER_H
