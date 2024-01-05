#include "TransactionConverter.h"
#include "TransactionRequestConverter.h"

namespace AptosRESTModel {

TransactionConverter::TransactionConverter() {}

Transaction TransactionConverter::ProcessTransactionContents(const nlohmann::json &item, const std::string &type)
{
    TransactionRequest transactionRequest = TransactionRequestConverter::ReadJson(item);
    Transaction transaction(transactionRequest);

    if (!type.empty())
    {
        transaction.setType(type);
    }

    if (!item["hash"].is_null())
    {
        transaction.setHash(item["hash"].get<std::string>());
    }

    if (!item["state_change_hash"].is_null())
    {
        transaction.setStateChangeHash(item["state_change_hash"].get<std::string>());
    }

    if (!item["event_root_hash"].is_null())
    {
        transaction.setEventRootHash(item["event_root_hash"].get<std::string>());
    }

    if (!item["state_checkpoint_hash"].is_null())
    {
        transaction.setStateCheckpointHash(item["state_checkpoint_hash"].get<std::string>());
    }

    if (!item["gas_used"].is_null())
    {
        transaction.setGasUsed(item["gas_used"].get<std::string>());
    }

    if (!item["success"].is_null())
    {
        transaction.setSuccess(item["success"].get<bool>());
    }

    if (!item["vm_status"].is_null())
    {
        transaction.setVmStatus(item["vm_status"].get<std::string>());
    }

    if (!item["accumulator_root_hash"].is_null())
    {
        transaction.setAccumulatorRootHash(item["accumulator_root_hash"].get<std::string>());
    }

    if (!item["events"].is_null())
    {
        std::vector<TransactionEvent> events;

        for (const auto& itemEvent : item["events"])
        {
            TransactionEvent eventTx = TransactionEvent::FromJson(itemEvent);
            events.push_back(eventTx);
        }

        transaction.setEvents(events);
    }

    return transaction;
}

Transaction TransactionConverter::ReadJson(const nlohmann::json &item)
{
    if (item.find("type") == item.end() && item.find("hash") != item.end())
    {
        if (item.find("vm_status") != item.end())
        {
            return ProcessTransactionContents(item);
        }
        else
        {
            TransactionRequest transactionRequest = TransactionRequestConverter::ReadJson(item);
            Transaction transaction(transactionRequest);
            transaction.setHash(item["hash"].get<std::string>());
            return transaction;
        }
    }
    else if (item.find("type") != item.end())
    {
        std::string type = item["type"].get<std::string>();

        if (type == "pending_transaction")
        {
            TransactionRequest transactionRequest = TransactionRequestConverter::ReadJson(item);
            Transaction transaction(transactionRequest);
            transaction.setType(type);

            if (item.find("hash") != item.end())
            {
                transaction.setHash(item["hash"].get<std::string>());
            }

            return transaction;
        }
        else if (type == "user_transaction")
        {
            return ProcessTransactionContents(item, type);
        }
    }

    // If none of the conditions are met, return a default-constructed Transaction
    return Transaction();
}

}
