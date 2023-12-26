#ifndef WALLET_H
#define WALLET_H
#define BOOST_ASIO_HAS_STD_INVOKE_RESULT
#include "../Accounts/Ed25519Bip32.h"
#include "../Accounts/Account.h"
#include "bip3x/bip3x_mnemonic.h"
#include "bip3x/bip3x_hdkey_encoder.h"
#include <memory>

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
    class Wallet
    {
    private:
        static const std::string DerivationPath;
        std::string wordList;
        std::string passphrase;
        /// <summary>
        /// The method used for <see cref="SeedMode.Ed25519Bip32"/> key generation.
        /// </summary>
        std::unique_ptr<Accounts::Ed25519Bip32> _ed25519Bip32;
        std::vector<uint8_t> _seed;
        SeedMode _seedMode;
        Accounts::Account _account;

        std::vector<uint8_t> toVector(const bip3x::bytes_data& data);

    public:
        std::vector<uint8_t> DeriveMnemonicSeed();
        void InitializeFirstAccount();
        Accounts::Account GetDerivedAccount(int index);
        void InitializeSeed();
        explicit Wallet(const std::string &mnemonicWords, const std::string &passphrase = "",
               SeedMode seedMode = SeedMode::Ed25519Bip32);
        Accounts::Account account() const;
        std::string getMnemonicsKey() const;
    };
}
#endif // WALLET_H
