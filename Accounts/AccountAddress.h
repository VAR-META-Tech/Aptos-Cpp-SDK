//
// Created by Anh NPH on 28/09/2023.
//

#ifndef APTOS_ACCOUNTADDRESS_H
#define APTOS_ACCOUNTADDRESS_H

#include "../BCS/BCSTypes.h"
#include <cryptopp/xed25519.h>
#include "../BCS/Serialization.h"
#include "../BCS/Deserialization.h"
#include "multipublickey.h"

struct AuthKeyScheme {
    static const CryptoPP::byte Ed25519 = 0x00;
    static const CryptoPP::byte MultiEd25519 = 0x01;
    static const CryptoPP::byte DeriveObjectAddressFromGuid = 0xFD;
    static const CryptoPP::byte DeriveObjectAddressFromSeed = 0xFE;
    static const CryptoPP::byte DeriveResourceAccountAddress = 0xFF;
};

class AccountAddress : public ISerializableTag{
private:
    CryptoPP::SecByteBlock _addressBytes;

public:
    static const int Length = 32;
    AccountAddress(const CryptoPP::SecByteBlock &addressBytes);
    std::string ToString() const override;
    static AccountAddress FromHex(std::string address);
    static AccountAddress FromKey(const CryptoPP::SecByteBlock &publicKey);
    static AccountAddress FromKey(const PublicKey &publicKey);

    void Serialize(Serialization &serializer) const override;
    static std::shared_ptr<ISerializableTag> Deserialize(Deserialization& deserializer);
    static AccountAddress ForNamedObject(const AccountAddress& creator, const CryptoPP::SecByteBlock &seed);
    static AccountAddress ForNamedToken(const AccountAddress& creator,
                                        const std::string& collectionName,
                                        const std::string& tokenName);
    static AccountAddress ForNamedCollection(AccountAddress creator, std::string collectionName);
    static AccountAddress ForResourceAccount(AccountAddress creator, const CryptoPP::SecByteBlock &seed);
    static AccountAddress FromMultiEd25519(MultiPublicKey keys);
    size_t GetHashCode() override;

    TypeTag Variant() const override;

    CryptoPP::SecByteBlock addressBytes() const;

private:
    bool IsSpecial() const;
};

bool operator==(const AccountAddress& lhs, const AccountAddress& rhs);

#endif //APTOS_ACCOUNTADDRESS_H
