#include "TransactionArgument.h"
#include "Serialization.h"

TransactionArgument::TransactionArgument(ISerializable *value, TypeTag encoderType)
    : value(value), encoderType(encoderType) {}

std::vector<uint8_t> TransactionArgument::Encode() {
    Serialization ser;
    ser.Serialize(value);
    return ser.GetBytes();
}
