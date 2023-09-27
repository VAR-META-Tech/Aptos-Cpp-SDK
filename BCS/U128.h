//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_U128_H
#define APTOS_U128_H

#include "BCSTypes.h"
#include <cryptopp/integer.h>

class U128  : public ISerializableTag{
private:
    CryptoPP::Integer value;

public:
    U128(CryptoPP::Integer value);

    void Serialize(Serialization& serializer);
    static U128* Deserialize(Deserialization& deserializer);
    TypeTag Variant();
    void* GetValue();
    std::string ToString() const;
    bool Equals(const U128& other) const;
    size_t GetHashCode() const;

    static CryptoPP::Integer Deserialize(const std::vector<uint8_t> &data);
};

#endif //APTOS_U128_H
