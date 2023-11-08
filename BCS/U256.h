//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_U256_H
#define APTOS_U256_H

#include "BCSTypes.h"
#include <cryptopp/integer.h>

class U256 : public ISerializableTag{
private:
    CryptoPP::Integer value;

public:
    U256(CryptoPP::Integer value);

    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializableTag> Deserialize(Deserialization& deserializer);
    TypeTag Variant() const override;
    CryptoPP::Integer GetValue() const;
    std::string ToString() const override;
    bool Equals(const U256& other) const;
    size_t GetHashCode() const;

    static CryptoPP::Integer Deserialize(const std::vector<uint8_t> &data);
};

#endif //APTOS_U256_H
