#include "wallet.h"
/*
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cryptopp/osrng.h>
#include <cryptopp/ed25519.h>
#include <cryptopp/sha.h>
#include <cryptopp/hmac.h>
#include <cryptopp/mnemonic.h>
#include <cryptopp/cryptlib.h>


Wallet::Wallet(CryptoPP::Mnemonic::WordCount wordCount, const CryptoPP::WordList &wordList, const std::string &passphrase, SeedMode seedMode)
    : _seedMode(seedMode), Passphrase(passphrase) {
    Mnemonic = CryptoPP::Mnemonic(wordList, wordCount);
    InitializeSeed();
}

Wallet::Wallet(const CryptoPP::Mnemonic &mnemonic, const std::string &passphrase, SeedMode seedMode)
    : Mnemonic(mnemonic), Passphrase(passphrase), _seedMode(seedMode) {
    InitializeSeed();
}

Wallet::Wallet(const std::string &mnemonicWords, const CryptoPP::WordList *wordList, const std::string &passphrase, SeedMode seedMode)
    : Passphrase(passphrase), _seedMode(seedMode) {
    Mnemonic = CryptoPP::Mnemonic(mnemonicWords, wordList ? *wordList : CryptoPP::WordList::English);
    InitializeSeed();
}

Wallet::Wallet(const byte *seed, size_t seedSize, const std::string &passphrase, SeedMode seedMode)
    : Passphrase(passphrase), _seedMode(seedMode) {
    if (seedSize != CryptoPP::Ed25519::ExpandedPrivateKeySizeInBytes)
        throw std::invalid_argument("Invalid seed length");

    _seed.assign(seed, seed + seedSize);
    InitializeFirstAccount();
}

Wallet::Account Wallet::GetAccount(int index) {
    if (_seedMode != SeedMode::Ed25519Bip32)
        throw std::runtime_error("Seed mode cannot derive Ed25519 based BIP32 keys");

    std::string path = DerivationPath;
    size_t found = path.find("x");
    if (found != std::string::npos) {
        path.replace(found, 1, std::to_string(index));
    }

    CryptoPP::Ed25519Bip32::Result result = Ed25519Bip32DerivePath(path);
    std::vector<byte> privateKey = std::get<0>(result);
    std::vector<byte> publicKey = std::get<1>(result);

    return Account(privateKey, publicKey);
}

void Wallet::InitializeFirstAccount() {
    if (_seedMode == SeedMode::Ed25519Bip32) {
        _ed25519Bip32.Initialize(_seed.data());
        Account = GetAccount(0);
    }
    else {
        std::vector<byte> privateKey, publicKey;
        EdKeyPairFromSeed(_seed, privateKey, publicKey);
        Account = Account(privateKey, publicKey);
    }
}

void Wallet::InitializeSeed() {
    _seed = DeriveMnemonicSeed();

    InitializeFirstAccount();
}

std::vector<byte> Wallet::DeriveMnemonicSeed() {
    if (!_seed.empty())
        return _seed;

    if (_seedMode == SeedMode::Ed25519Bip32) {
        return Mnemonic.DeriveSeed();
    }
    else if (_seedMode == SeedMode::Bip39) {
        return Mnemonic.DeriveSeed(Passphrase);
    }
    else {
        return Mnemonic.DeriveSeed();
    }
}

CryptoPP::Ed25519Bip32::Result Wallet::Ed25519Bip32DerivePath(const std::string &path) {
    std::vector<byte> account, chainCode;
    _ed25519Bip32.DerivePath(path, account, chainCode);
    std::vector<byte> privateKey, publicKey;
    EdKeyPairFromSeed(account, privateKey, publicKey);

    return std::make_tuple(privateKey, publicKey);
}

*/
