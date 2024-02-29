//
// Created by Anh NPH on 28/09/2023.
//

#include "Account.h"
#include <cryptopp/xed25519.h>
#include <cryptopp/osrng.h>
#include "../HDWallet/Utils/Utils.h"
#include <cryptopp/cryptlib.h>
#include "AuthenticationKey.h"

namespace Aptos::Accounts
{

    Account::Account()
    {
        CryptoPP::AutoSeededRandomPool rng;
        CryptoPP::ed25519PrivateKey privateKey;
        privateKey.GenerateRandom(rng, CryptoPP::g_nullNameValuePairs);
        CryptoPP::ed25519PublicKey publicKey;
        privateKey.MakePublicKey(publicKey);
        CryptoPP::SecByteBlock privateKeyBytes(privateKey.GetPrivateKeyBytePtr(), Utils::Ed25519PrivateKeySeedSizeInBytes);
        CryptoPP::SecByteBlock publicKeyBytes(publicKey.GetPublicKeyBytePtr(), Utils::Ed25519PublicKeySizeInBytes);
        m_privateKey = std::make_shared<PrivateKey>(privateKeyBytes);
        m_publicKey = std::make_shared<PublicKey>(publicKeyBytes);
        m_accountAddress = std::make_shared<AccountAddress>(AccountAddress::FromKey(publicKeyBytes));
    }

    Account::Account(std::string privateKeyStr, std::string publicKeyStr)
    {
        m_privateKey = std::make_shared<PrivateKey>(privateKeyStr);
        m_publicKey = std::make_shared<PublicKey>(publicKeyStr);
        m_accountAddress = std::make_shared<AccountAddress>(AccountAddress::FromKey(*m_publicKey));
    }

    Account::Account(const CryptoPP::SecByteBlock &privateKeyBytes, const CryptoPP::SecByteBlock &publicKeyBytes)
    {
        m_privateKey = std::make_shared<PrivateKey>(privateKeyBytes);
        m_publicKey = std::make_shared<PublicKey>(publicKeyBytes);
        m_accountAddress = std::make_shared<AccountAddress>(AccountAddress::FromKey(*m_publicKey));
    }

    Account Account::LoadKey(std::string privateKeyHex)
    {
        PrivateKey privateKey(privateKeyHex);
        auto publicKey = privateKey.GetPublicKey();
        return Account(privateKey.ToString(), publicKey.ToString());
    }

    std::string Account::AuthKey()
    {
        auto authkey = AuthenticationKey::FromEd25519PublicKey(m_publicKey->KeyBytes());
        return authkey.DerivedAddress();
    }

    bool Account::Verify(const CryptoPP::SecByteBlock &message, Ed25519Signature signature)
    {
        return m_publicKey->Verify(message, signature);
    }

    Ed25519Signature Account::Sign(CryptoPP::SecByteBlock message)
    {
        return m_privateKey->Sign(message);
    }

    std::shared_ptr<PrivateKey> Account::getPrivateKey() const
    {
        return m_privateKey;
    }

    std::shared_ptr<PublicKey> Account::getPublicKey() const
    {
        return m_publicKey;
    }

    std::shared_ptr<AccountAddress> Account::getAccountAddress() const
    {
        return m_accountAddress;
    }

}