//
// Created by Anh NPH on 28/09/2023.
//

#ifndef APTOS_ACCOUNT_H
#define APTOS_ACCOUNT_H

#include <stdlib.h>
#include <cstring>
#include "PrivateKey.h"
#include "PublicKey.h"
#include "AccountAddress.h"
#include <memory>

class Account {
public:
    Account();
    Account(std::string privateKeyStr, std::string publicKeyStr);
    Account(const CryptoPP::SecByteBlock& privateKeyBytes, const CryptoPP::SecByteBlock& publicKeyBytes);
    static Account LoadKey(std::string privateKeyHex);
    std::string AuthKey();    
    bool Verify(const CryptoPP::SecByteBlock& message, Signature signature);
    Signature Sign(CryptoPP::SecByteBlock message);
    std::shared_ptr<PrivateKey> getPrivateKey() const;
    std::shared_ptr<PublicKey> getPublicKey() const;
    std::shared_ptr<AccountAddress> getAccountAddress() const;

private:
    std::shared_ptr<PrivateKey> m_privateKey;
    std::shared_ptr<PublicKey> m_publicKey;
    std::shared_ptr<AccountAddress> m_accountAddress;
};

#endif //APTOS_ACCOUNT_H
