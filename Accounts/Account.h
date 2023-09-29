//
// Created by Anh NPH on 28/09/2023.
//

#ifndef APTOS_ACCOUNT_H
#define APTOS_ACCOUNT_H

#include <vector>
#include <string>
#include "PrivateKey.h"
#include "PublicKey.h"
#include "AccountAddress.h"
#include "AuthenticationKey.h"
#include "Signature.h"
#include <cryptopp/xed25519.h>

class Account
{
public:
    Account();
    Account(const std::string& privateKey, const std::string& publicKey);
    Account(const std::vector<CryptoPP::byte>& privateKey, const std::vector<CryptoPP::byte>& publicKey);

    static Account Generate();
    static Account LoadKey(const std::string& privateKeyHex);

    std::string AuthKey();
    bool Verify(const std::vector<CryptoPP::byte>& message, const Signature& signature);
    Signature Sign(const std::vector<CryptoPP::byte>& message);

    static std::pair<std::vector<CryptoPP::byte>, std::vector<CryptoPP::byte>> EdKeyPairFromSeed(const std::vector<CryptoPP::byte>& seed);

private:
//    PrivateKey _privateKey;
//    PublicKey _publicKey;
//    AccountAddress _accountAddress;
//    std::vector<CryptoPP::byte> _privateKeyShort;
};


#endif //APTOS_ACCOUNT_H
