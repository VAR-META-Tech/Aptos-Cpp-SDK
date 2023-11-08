//
// Created by Anh NPH on 28/09/2023.
//

#ifndef APTOS_AUTHENTICATIONKEY_H
#define APTOS_AUTHENTICATIONKEY_H

#include "Types/MultiEd25519PublicKey.h"
#include <cryptopp/xed25519.h>

class AuthenticationKey {
public:
    static const int LENGTH = 32;
    static const CryptoPP::byte MULTI_ED25519_SCHEME = 0x01;
    static const CryptoPP::byte ED25519_SCHEME = 0x00;
    static const CryptoPP::byte DERIVE_RESOURCE_ACCOUNT_SCHEME = 255;

    AuthenticationKey(const CryptoPP::SecByteBlock &bytes);

    static AuthenticationKey FromMultiEd25519PublicKey(Aptos::Accounts::Types::MultiEd25519PublicKey publicKey);

    static AuthenticationKey FromEd25519PublicKey(const CryptoPP::SecByteBlock &publicKey);

    std::string DerivedAddress();

private:
    CryptoPP::SecByteBlock _bytes;

};

#endif //APTOS_AUTHENTICATIONKEY_H
