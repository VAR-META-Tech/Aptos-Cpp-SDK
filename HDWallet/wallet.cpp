#include "wallet.h"
#include "Utils/Utils.h"

namespace Aptos::HDWallet
{
    const std::string Wallet::DerivationPath = "m/44'/637'/x'/0'/0'";
    Account Wallet::account() const
    {
        return _account;
    }

    libbitcoin::data_chunk Wallet::DeriveMnemonicSeed()
    {
        auto hd_seed = bc::wallet::decode_mnemonic(wordList);
        return bc::to_chunk(hd_seed);
    }

    void Wallet::InitializeFirstAccount()
    {
        _ed25519Bip32 = std::make_unique<Ed25519Bip32>(_seed);
        _account = GetDerivedAccount(0);
    }

    Account Wallet::GetDerivedAccount(int index)
    {
        if (_seedMode != SeedMode::Ed25519Bip32)
        {
            throw std::runtime_error("Seed mode cannot derive Ed25519 based BIP32 keys");
        }

        std::string path = DerivationPath;
        size_t found = path.find("x");
        if (found != std::string::npos)
        {
            path.replace(found, 1, std::to_string(index));
        }
        std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> account = _ed25519Bip32->DerivePath(path);
        std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> keyPair = Utils::EdKeyPairFromSeed(account.first);

        return Account(keyPair.first, keyPair.second);
    }

    void Wallet::InitializeSeed()
    {
        _seed = DeriveMnemonicSeed();
        InitializeFirstAccount();
    }

    Wallet::Wallet(const std::string &mnemonicWords, SeedMode seedMode)
        : _seedMode(seedMode)
    {
        wordList = bc::split(mnemonicWords, " ", true);
        //    wordList = bc::wallet::create_mnemonic(my_word_list, bc::wallet::language::en);
        InitializeSeed();
    }
}