#ifndef TRANSACTIONREQUESTCONVERTER_H
#define TRANSACTIONREQUESTCONVERTER_H

#include "TransactionRequest.h"

namespace AptosRESTModel {

class TransactionRequestConverter
{
public:
    static AptosRESTModel::TransactionRequest ReadJson(const nlohmann::json& jsonData);
    static nlohmann::json WriteJson(const AptosRESTModel::TransactionRequest &transactionRequest);
};

}
#endif // TRANSACTIONREQUESTCONVERTER_H
