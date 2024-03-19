#include "Wallet.h"
#include "Utils/Utils.h"

using namespace Aptos::Accounts;
namespace Aptos::HDWallet
{
    const std::string Wallet::DerivationPath = "m/44'/637'/x'/0'/0'";
    Account Wallet::Account() const
    {
        return _account;
    }

    std::vector<uint8_t> Wallet::DeriveMnemonicSeed()
    {
        std::vector<uint8_t> out;
        auto res = bip3x::bip3x_mnemonic::validate_words("en", wordList.c_str());
        if (!res) {
            throw std::runtime_error("Wrong wallet mnemonic seed");
        }
        bip3x::bytes_64 seed = bip3x::bip3x_hdkey_encoder::make_bip39_seed(wordList);
        out = toVector(seed);
        return out;
    }

    std::vector<uint8_t> Wallet::toVector(const bip3x::bytes_data &data)
    {

        std::vector<uint8_t> result;
        result.reserve(data.size());

        for (size_t i = 0; i < data.size(); i++)
        {
            result.push_back(data[i]);
        }

        return result;
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

        return Accounts::Account(keyPair.first, keyPair.second);
    }

    void Wallet::InitializeSeed()
    {
        _seed = DeriveMnemonicSeed();
        InitializeFirstAccount();
    }

    Wallet::Wallet(const std::string &mnemonicWords, const std::string &passphrase, SeedMode seedMode)
        : wordList(mnemonicWords), passphrase(passphrase), _seedMode(seedMode)
    {
        InitializeSeed();
    }

    std::string Wallet::getMnemonicsKey() const
    {
        return wordList;
    }
}
