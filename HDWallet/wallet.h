#ifndef WALLET_H
#define WALLET_H

#include "../Accounts/Account.h"
/*
class Wallet {
public:
    enum class SeedMode {
        Ed25519Bip32,
        Bip39
    };

    Wallet(CryptoPP::Mnemonic::WordCount wordCount, const CryptoPP::WordList& wordList, const std::string& passphrase = "", SeedMode seedMode = SeedMode::Ed25519Bip32);

    Wallet(const CryptoPP::Mnemonic& mnemonic, const std::string& passphrase = "", SeedMode seedMode = SeedMode::Ed25519Bip32);

    Wallet(const std::string& mnemonicWords, const CryptoPP::WordList* wordList = nullptr, const std::string& passphrase = "", SeedMode seedMode = SeedMode::Ed25519Bip32);

    Wallet(const byte* seed, size_t seedSize, const std::string& passphrase = "", SeedMode seedMode = SeedMode::Ed25519Bip32);

    Account GetAccount(int index);

private:
    const std::string DerivationPath = "m/44'/637'/x'/0'/0'";
    const std::string Passphrase;
    SeedMode _seedMode;
    std::vector<byte> _seed;
    CryptoPP::Ed25519Bip32 _ed25519Bip32;
    CryptoPP::Mnemonic Mnemonic;

    void InitializeFirstAccount();

    void InitializeSeed();

    std::vector<byte> DeriveMnemonicSeed();

    CryptoPP::Ed25519Bip32::Result Ed25519Bip32DerivePath(const std::string& path);
public:
    Account Account;
};
*/
#endif // WALLET_H
