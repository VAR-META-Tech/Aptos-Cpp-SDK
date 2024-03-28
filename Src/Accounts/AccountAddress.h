//
// Created by Anh NPH on 28/09/2023.
//

#ifndef APTOS_ACCOUNTADDRESS_H
#define APTOS_ACCOUNTADDRESS_H

#include "../BCS/BCS.h"
#include <cryptopp/xed25519.h>
#include "Multipublickey.h"
using namespace Aptos::Accounts::Ed25519;

namespace Aptos::Accounts
{
    /// <summary>
    /// A class representing the available authorization key schemes for Aptos Blockchain accounts.
    /// </summary>
    struct AuthKeyScheme
    {

        /// <summary>
        /// The ED25519 authorization key scheme value.
        /// </summary>
        static const CryptoPP::byte Ed25519 = 0x00;

        /// <summary>
        /// The multi-ED25519 authorization key scheme value.
        /// </summary>
        static const CryptoPP::byte MultiEd25519 = 0x01;

        /// <summary>
        /// The authorization key scheme value used to derive an object address from a GUID.
        /// </summary>
        static const CryptoPP::byte DeriveObjectAddressFromGuid = 0xFD;

        /// <summary>
        /// The authorization key scheme value used to derive an object address from a seed.
        /// </summary>
        static const CryptoPP::byte DeriveObjectAddressFromSeed = 0xFE;

        /// <summary>
        /// The authorization key scheme value used to derive a resource account address.
        /// </summary>
        static const CryptoPP::byte DeriveResourceAccountAddress = 0xFF;
    };

    /// <summary>
    /// Represents an Aptos account address.
    /// More details can her found <see cref="https://aptos.dev/concepts/accounts">here</see>.
    /// </summary>
    class APTOS_API AccountAddress : public BCS::ISerializableTag
    {
    private:
        CryptoPP::SecByteBlock _addressBytes;

    public:
        /// <summary>
        /// The length of the data in bytes.
        /// </summary>
        static const int Length = 32;

        /// <summary>
        /// Initializes an account address by setting a 32-byte representation of an address.
        /// </summary>
        /// <param name="addressBytes">Byte array representing address.</param>
        explicit AccountAddress(const CryptoPP::SecByteBlock &addressBytes);
        AccountAddress() = default;
        /// <summary>
        /// Represent an account address in a way that is compliant with the v1 address
        /// standard.The standard is defined as part of AIP-40, read more here:
        /// https://github.com/aptos-foundation/AIPs/blob/main/aips/aip-40.md
        /// </summary>
        /// <returns>String representation of account address</returns>
        std::string ToString() const override;

        /// <summary>
        /// Returns an AccountAddress object from a hexadecimal Address.
        /// </summary>
        /// <param name="address">Hexadecimal representation of an Address.</param>
        /// <returns>An account address object</returns>
        static AccountAddress FromHex(std::string address);

        /// <summary>
        /// Generate an AccountAddress object from a given public key, byte array.
        /// </summary>
        /// <param name="publicKey">Byte array representing a public key.</param>
        /// <returns>An account address object.</returns>
        static AccountAddress FromKey(const CryptoPP::SecByteBlock &publicKey);
        static AccountAddress FromKey(const PublicKey &publicKey);

        /// <summary>
        /// Reference implementation to BCS can be found <see cref="https://github.com/aptos-labs/bcs">here</see>.
        /// More details on creating a BCS signed transaction can be found <see cref="https://aptos.dev/guides/creating-a-signed-transaction/#bcs">here</see>.
        ///
        /// Binary Canonical Serialization (BCS) is a serialization format applied to the raw (unsigned) transaction.
        /// See BCS for a description of the design goals of BCS.
        /// BCS is not a self-describing format.As such, in order to deserialize a message,
        /// one must know the message type and layout ahead of time.
        /// </summary>
        void Serialize(BCS::Serialization &serializer) const override;
        static std::shared_ptr<BCS::ISerializableTag> Deserialize(BCS::Deserialization &deserializer);

        /// <summary>
        /// Create an AccountAddress instance for a named object.
        ///
        /// This function creates an AccountAddress instance for a named object given the creator's address and a seed value. The function generates a new address by concatenating the byte representation
        /// of the creator's address, the provided seed value, and the DERIVE_OBJECT_ADDRESS_FROM_SEED authorization key scheme value. It then computes the SHA3-256 hash of the resulting byte
        /// array to generate a new AccountAddress instance.
        /// </summary>
        /// <param name="creator">An AccountAddress instance representing the address of the account that will create the named object.</param>
        /// <param name="seed">A byte array used to create a unique named object.</param>
        /// <returns>An AccountAddress instance representing the newly created named object.</returns>
        static AccountAddress ForNamedObject(const AccountAddress &creator, const CryptoPP::SecByteBlock &seed);

        /// <summary>
        /// Generates an AccountAddress for a named token by concatenating the collectionName, the tokenName, and the separator "::"
        /// as a Data and calling the forNamedObject function with the resulting Data as the seed.
        /// </summary>
        /// <param name="creator">The AccountAddress of the account that creates the token./param>
        /// <param name="collectionName">A String that represents the name of the collection to which the token belongs.</param>
        /// <param name="tokenName">A String that represents the name of the token.</param>
        /// <returns>An AccountAddress object that represents the named token.</returns>
        static AccountAddress ForNamedToken(const AccountAddress &creator,
                                            const std::string &collectionName,
                                            const std::string &tokenName);

        /// <summary>
        /// Derive an AccountAddress for a named collection.
        ///
        /// This function takes the creator's AccountAddress and the name of the collection as a String. The collection name is
        /// then converted to data using UTF-8 encoding. The forNamedObject function is called with the creator's address and the
        /// collection name data as the seed. This returns an AccountAddress derived from the creator's address and collection name
        /// seed, which represents the named collection.
        /// </summary>
        /// <param name="creator">The creator's AccountAddress.</param>
        /// <param name="collectionName">The name of the collection as a String.</param>
        /// <returns>An AccountAddress that represents the named collection.</returns>
        static AccountAddress ForNamedCollection(AccountAddress creator, std::string collectionName);

        /// <summary>
        /// Create an AccountAddress instance for a resource account.
        ///
        /// This function creates an AccountAddress instance for a resource account given the creator's address and a seed value. The function generates a new address by concatenating the byte
        /// representation of the creator's address, the provided seed value, and the DERIVE_RESOURCE_ACCOUNT_ADDRESS authorization key scheme value. It then computes the SHA3-256
        /// hash of the resulting byte array to generate a new AccountAddress instance.
        /// </summary>
        /// <param name="creator">An AccountAddress instance representing the address of the account that will create the resource account.</param>
        /// <param name="seed">A byte array used to create a unique resource account.</param>
        /// <returns>An AccountAddress instance representing the newly created resource account.</returns>
        static AccountAddress ForResourceAccount(AccountAddress creator, const CryptoPP::SecByteBlock &seed);

        /// <summary>
        /// Create an AccountAddress instance from a MultiPublicKey.
        ///
        /// This function creates an AccountAddress instance from a provided MultiPublicKey. The function generates a new address by appending the Multi ED25519 authorization key
        /// scheme value to the byte representation of the provided MultiPublicKey, and then computes the SHA3-256 hash of the resulting byte array. The resulting hash is used to create
        /// a new AccountAddress instance.
        /// </summary>
        /// <param name="keys">A MultiPublicKey instance representing the multiple public keys to create the AccountAddress instance from.</param>
        /// <returns>An AccountAddress instance created from the provided MultiPublicKey.</returns>
        static AccountAddress FromMultiEd25519(MultiPublicKey keys);
        size_t GetHashCode() const override;

        BCS::TypeTag Variant() const override;

        /// <summary>
        /// The address data itself represented in byte array formatting.
        /// </summary>
        CryptoPP::SecByteBlock addressBytes() const;

        /// <summary>
        /// Returns whether the address is a "special" address. Addresses are considered
        /// special if the first 63 characters of the hex string are zero.In other words,
        /// an address is special if the first 31 bytes are zero and the last byte is
        /// smaller than `0b10000` (16). In other words, special is defined as an address
        /// that matches the following regex: `^0x0{63}[0-9a-f]$`. In short form this means
        /// the addresses in the range from `0x0` to `0xf` (inclusive) are special.
        ///
        /// For more details see the v1 address standard defined as part of AIP-40:
        /// https://github.com/aptos-foundation/AIPs/blob/main/aips/aip-40.md
        /// </summary>
        /// <returns>Boolean representing if the address is special or not.</returns>
    private:
        bool IsSpecial() const;
    };

    bool APTOS_API operator==(const AccountAddress &lhs, const AccountAddress &rhs);
}
#endif // APTOS_ACCOUNTADDRESS_H
