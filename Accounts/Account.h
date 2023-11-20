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

namespace Aptos::Accounts
{
    /// <summary>
    /// Represents an Aptos Accounts.
    /// An Aptos account is represented by an extended private key,
    /// a public key and it's authentication key.
    /// </summary>
    class Account
    {
    public:
        /// <summary>
        /// Create a new Aptos Account.
        /// </summary>
        Account();

        /// <summary>
        /// </summary>
        /// Initialize an account with a given private and public keys.
        /// <param name="privateKeyStr">The private key.</param>
        /// <param name="publicKeyStr">The public key.</param>
        Account(std::string privateKeyStr, std::string publicKeyStr);

        /// <inheritdoc cref="Account(string, string)"/>
        Account(const CryptoPP::SecByteBlock &privateKeyBytes, const CryptoPP::SecByteBlock &publicKeyBytes);

        /// <summary>
        /// Creates an account from private key in string format.
        /// </summary>
        /// <param name="privateKeyHex">The private key.</param>
        /// <returns>A new account.</returns>
        static Account LoadKey(std::string privateKeyHex);

        /// <summary>
        /// Returns the Authentication Key for the associated account.
        /// </summary>
        /// <returns>String representation of the authentication key</returns>
        std::string AuthKey();

        /// <summary>
        /// Verify a given signed message with the current account's public key
        /// </summary>
        /// <param name="message">The signed message.</param>
        /// <param name="signature">The signature of the message.</param>
        /// <returns>True is the signature is valid, False otherwise</returns>
        bool Verify(const CryptoPP::SecByteBlock &message, Signature signature);

        /// <summary>
        /// Sign a given byte array (data) with the current account's private key
        /// </summary>
        /// <param name="message"></param> The signature of the data.
        /// <returns>The signature as an object</returns>
        Signature Sign(CryptoPP::SecByteBlock message);

        /// <summary>
        /// Private key representation.
        /// </summary>
        std::shared_ptr<PrivateKey> getPrivateKey() const;

        /// <summary>
        /// Public key representation.
        /// </summary>
        std::shared_ptr<PublicKey> getPublicKey() const;

        /// <summary>
        /// Account address representation.
        /// </summary>
        std::shared_ptr<AccountAddress> getAccountAddress() const;

    private:
        std::shared_ptr<PrivateKey> m_privateKey;
        std::shared_ptr<PublicKey> m_publicKey;
        std::shared_ptr<AccountAddress> m_accountAddress;
    };
}
#endif // APTOS_ACCOUNT_H
