//
// Created by Anh NPH on 28/09/2023.
//

#ifndef APTOS_ACCOUNTADDRESS_H
#define APTOS_ACCOUNTADDRESS_H

#include "../BCS/BCSTypes.h"
#include <cryptopp/xed25519.h>
#include "../BCS/Serialization.h"
#include "../BCS/Deserialization.h"

struct AuthKeyScheme {
    static const CryptoPP::byte Ed25519 = 0x00;
    static const CryptoPP::byte MultiEd25519 = 0x01;
    static const CryptoPP::byte DeriveObjectAddressFromGuid = 0xFD;
    static const CryptoPP::byte DeriveObjectAddressFromSeed = 0xFE;
    static const CryptoPP::byte DeriveResourceAccountAddress = 0xFF;
};

class AccountAddress : public ISerializableTag{
private:
    std::vector<CryptoPP::byte> _addressBytes;

public:
    static const int Length = 32;
    AccountAddress(std::vector<CryptoPP::byte>& addressBytes);
    std::string ToString();
    static AccountAddress FromHex(std::string address);
    static AccountAddress FromKey(std::vector<CryptoPP::byte>& publicKey);

    void Serialize(Serialization &serializer) override;
    static AccountAddress Deserialize(Deserialization& deserializer);

    size_t GetHashCode() override;

    TypeTag Variant() override;

    void *GetValue() override;

private:
    bool IsSpecial();
};

#endif //APTOS_ACCOUNTADDRESS_H
