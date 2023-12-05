#ifndef WALLET_H
#define WALLET_H
#include "../Accounts/Ed25519Bip32.h"
#include "../Accounts/Account.h"
#include <bitcoin/system.hpp>
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
    class Wallet
    {
    private:
        static const std::string DerivationPath;
        bc::wallet::word_list wordList;
        bc::wallet::hd_private privateKey;
        std::string passphrase;
        /// <summary>
        /// The method used for <see cref="SeedMode.Ed25519Bip32"/> key generation.
        /// </summary>
        std::unique_ptr<Ed25519Bip32> _ed25519Bip32;
        std::vector<std::uint8_t> _seed;
        SeedMode _seedMode;
        Account _account;

    public:
        bc::data_chunk DeriveMnemonicSeed();
        void InitializeFirstAccount();
        Account GetDerivedAccount(int index);
        void InitializeSeed();
        Wallet(const std::string &mnemonicWords,
               SeedMode seedMode = SeedMode::Ed25519Bip32);
        Account account() const;
        std::string getMnemonicsKey() const;
    };
}
#endif // WALLET_H
