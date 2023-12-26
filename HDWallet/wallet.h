#ifndef WALLET_H
#define WALLET_H
#define BOOST_ASIO_HAS_STD_INVOKE_RESULT
#include "../Accounts/Ed25519Bip32.h"
#include "../Accounts/Account.h"
#include "bip3x/bip3x_mnemonic.h"
#include "bip3x/bip3x_hdkey_encoder.h"
#include <memory>

using namespace Aptos::Accounts;
namespace Aptos::HDWallet
{
    /// <summary>
    /// Specifies the available seed modes for key generation.
    /// <remarks>
    /// Available modes:
    /// <para> <see cref="SeedMode.Ed25519Bip32"/> </para>
    /// <para> <see cref="SeedMode.Bip39"/> </para>
    /// </remarks>
    /// </summary>
    enum class SeedMode
    {
        /// <summary>
        /// Generates Ed25519 based BIP32 keys.
        /// <remarks>This seed mode is compatible with the keys generated in the Sollet/SPL Token Wallet, it does not use a passphrase to harden the mnemonic seed.</remarks>
        /// </summary>
        Ed25519Bip32,
        /// <summary>
        /// Generates BIP39 keys.
        /// <remarks>This seed mode is compatible with the keys generated in the solana-keygen cli, it uses a passphrase to harden the mnemonic seed.</remarks>
        /// </summary>
        Bip39
    };

    /// <summary>
    /// Represents an Aptos wallet.
    /// </summary>
    class Wallet
    {
    private:
        /// <summary>
        /// The derivation path.
        /// </summary>
        static const std::string DerivationPath;

        std::string wordList;

        /// <summary>
        /// The passphrase string.
        /// </summary>
        std::string passphrase;

        /// <summary>
        /// The method used for <see cref="SeedMode.Ed25519Bip32"/> key generation.
        /// </summary>
        std::unique_ptr<Ed25519Bip32> _ed25519Bip32;

        /// <summary>
        /// The seed derived from the mnemonic and/or passphrase.
        /// </summary>
        std::vector<uint8_t> _seed;

        /// <summary>
        /// The seed mode used for key generation.
        /// Aptos currently supports BIP39
        /// </summary>
        SeedMode _seedMode;

        /// <summary>
        /// The key pair.
        /// </summary>
        Account _account;

        std::vector<uint8_t> toVector(const bip3x::bytes_data &data);

    public:
        /// <summary>
        /// Derive a seed from the passed mnemonic and/or passphrase, depending on <see cref="SeedMode"/>.
        /// </summary>
        /// <returns>The seed.</returns>
        std::vector<uint8_t> DeriveMnemonicSeed();

        /// <summary>
        /// Initializes the first account with a key pair derived from the initialized seed.
        /// </summary>
        void InitializeFirstAccount();

        /// <summary>
        /// Gets the account at the passed index using the ed25519 bip32 derivation path.
        /// </summary>
        /// <param name="index">The index of the account.</param>
        /// <returns>The account.</returns>
        Account GetDerivedAccount(int index);

        /// <summary>
        /// Derive the mnemonic seed and generate the key pair for the configured wallet.
        /// </summary>
        void InitializeSeed();

        /// <summary>
        /// Initialize a wallet from the passed mnemonic and passphrase.
        /// </summary>
        /// <param name="mnemonic">The mnemonic.</param>
        /// <param name="passphrase">The passphrase.</param>
        /// <param name="seedMode">The seed generation mode.</param>
        explicit Wallet(const std::string &mnemonicWords, const std::string &passphrase = "",
                        SeedMode seedMode = SeedMode::Ed25519Bip32);
        Account account() const;

        /// <summary>
        /// The mnemonic words.
        /// </summary>
        // ReSharper disable once MemberCanBePrivate.Global
        std::string getMnemonicsKey() const;
    };
}
#endif // WALLET_H
