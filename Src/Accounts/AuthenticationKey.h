//
// Created by Anh NPH on 28/09/2023.
//

#ifndef APTOS_AUTHENTICATIONKEY_H
#define APTOS_AUTHENTICATIONKEY_H

#include "Types/MultiEd25519PublicKey.h"
#include <cryptopp/xed25519.h>
namespace Aptos::Accounts
{
    /// <summary>
    /// Represents an Authentication Key \n
    /// During the account creation process, a 32-byte authentication key comes to exist first. \n
    /// This authentication key is then returned as it is as the 32-byte account Address. \n
    ///
    /// NOTE: Generating the authentication key for an account requires that you provide one of  \n
    /// the below 1-byte signature scheme identifiers for this account, i.e., \n
    /// whether the account is a single signature or a multisig account: \n
    /// More info on account addresses found <see cref="https://aptos.dev/concepts/accounts/#account-Address">here</see>.
    /// </summary>
    class APTOS_API AuthenticationKey
    {
    public:
        /// <summary>
        /// Byte length of authentication key.
        /// </summary>
        static const int LENGTH = 32;

        /// <summary>
        /// Represents various Ed25519 and resource account derivation schemes used in the Aptos blockchain.
        /// </summary>
        enum Scheme : CryptoPP::byte { 
            /// <summary>
            /// Denotes the multi-Ed25519 authentication scheme.
            /// </summary>
            MULTI_ED25519 = 0x01,

            /// <summary>
            /// Denotes the single-key Ed25519 authentication scheme.
            /// </summary>
            ED25519 = 0x00,

            /// <summary>
            /// Denotes the scheme used for deriving resource accounts.
            /// </summary>
            DERIVE_RESOURCE_ACCOUNT = 255
        };
        /// <summary>
        /// Initialize the Authentication Key.
        /// </summary>
        /// <param name="bytes">The raw byte array of the key itself.</param>
        /// <exception cref="ArgumentException"></exception>
        explicit AuthenticationKey(const CryptoPP::SecByteBlock &bytes);

        /// <summary>
        /// Converts a K-of-N MultiEd25519PublicKey to AuthenticationKey with: \n
        /// `auth_key = sha3-256(p_1 | � | p_n | K | 0x01)`. `K` represents the K-of-N required for \n
        /// authenticating the transaction. `0x01` is the 1-byte scheme for multisig. \n
        /// </summary>
        /// <returns>Authentication key object from a multi ED25519 key</returns>
        static AuthenticationKey FromMultiEd25519PublicKey(Aptos::Accounts::Types::MultiEd25519PublicKey publicKey);

        /// <summary>
        /// Converts single Public Key (bytes) into Authentication Key \n
        /// auth_key = sha3-256(pubkey_A | 0x00) \n
        /// where | denotes concatenation. The 0x00 is the 1-byte single-signature scheme identifier. \n
        /// </summary>
        /// <param name="publicKey">Publick key, in byte array format, used to generate the authentication key</param>
        /// <returns>Authentication key object</returns>
        static AuthenticationKey FromEd25519PublicKey(const CryptoPP::SecByteBlock &publicKey);

        /// <summary>
        /// Derives hexadecimal Address from authentication key array of bytes.
        /// </summary>
        /// <returns>A hexa string representation Address of the authentication key</returns>
        std::string DerivedAddress();

    private:
        /// <summary>
        /// Byte array representing authentication key.
        /// </summary>
        CryptoPP::SecByteBlock _bytes;
    };
}
#endif // APTOS_AUTHENTICATIONKEY_H