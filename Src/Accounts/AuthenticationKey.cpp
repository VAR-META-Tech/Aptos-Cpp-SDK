//
// Created by Anh NPH on 28/09/2023.
//

#include "AuthenticationKey.h"
#include <cryptopp/hex.h>
#include <cryptopp/sha3.h>
#include <iomanip>
namespace Aptos::Accounts
{
    AuthenticationKey::AuthenticationKey(const CryptoPP::SecByteBlock &bytes)
    {
        if (bytes.size() != LENGTH)
            throw std::invalid_argument("Byte array must be " + std::to_string(LENGTH) + " bytes");
        _bytes = bytes;
    }

    AuthenticationKey AuthenticationKey::FromMultiEd25519PublicKey(Aptos::Accounts::Types::MultiEd25519PublicKey publicKey)
    {
        CryptoPP::SHA3_256 sha3;
        CryptoPP::SecByteBlock pubKeyBytes = publicKey.ToBytes();
        CryptoPP::SecByteBlock bytes;
        bytes.resize(pubKeyBytes.size() + 1);
        std::ranges::copy(pubKeyBytes.begin(), pubKeyBytes.end(), bytes.begin());
        bytes[pubKeyBytes.size()] = AuthenticationKey::Scheme::MULTI_ED25519;
        sha3.Update(bytes.data(), bytes.size());
        CryptoPP::SecByteBlock result;
        result.resize(CryptoPP::ed25519PublicKey::PUBLIC_KEYLENGTH);
        sha3.Final(result.data());
        return AuthenticationKey(result);
    }

    AuthenticationKey AuthenticationKey::FromEd25519PublicKey(const CryptoPP::SecByteBlock &publicKey)
    {
        CryptoPP::SHA3_256 sha3;
        CryptoPP::SecByteBlock bytes;
        bytes.resize(CryptoPP::ed25519PublicKey::PUBLIC_KEYLENGTH + 1);
        std::ranges::copy(publicKey.begin(), publicKey.end(), bytes.begin());
        bytes[CryptoPP::ed25519PublicKey::PUBLIC_KEYLENGTH] = AuthenticationKey::Scheme::ED25519;
        sha3.Update(bytes.data(), bytes.size());
        CryptoPP::SecByteBlock result;
        result.resize(CryptoPP::ed25519PublicKey::PUBLIC_KEYLENGTH);
        sha3.Final(result.data());
        return AuthenticationKey(result);
    }

    std::string AuthenticationKey::DerivedAddress()
    {
        std::ostringstream ss;
        ss << std::hex;

        for (const auto &byte : _bytes)
        {
            ss << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }

        return "0x" + ss.str();
    }
}