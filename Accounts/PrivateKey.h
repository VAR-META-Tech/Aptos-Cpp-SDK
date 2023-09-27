//
// Created by Anh NPH on 26/09/2023.
//

#ifndef APTOS_PRIVATEKEY_H
#define APTOS_PRIVATEKEY_H

#include <string>
#include <vector>
#include "Signature.h"
#include "PublicKey.h"

class PrivateKey {
public:

    static const int KeyLength = 32;
    static const int ExtendedKeyLength = 64;

    PrivateKey() = default;
    PrivateKey(const std::string& hexKey);
    PrivateKey(const std::vector<uint8_t>& seed);

    std::vector<uint8_t> Sign(const std::vector<uint8_t>& message) const;

    std::string GetKeyHex() const;

    PublicKey GetPublicKey() const;

    bool operator==(const PrivateKey& rhs) const;

    static PrivateKey Random();

    friend std::ostream& operator<<(std::ostream& os, const PrivateKey& key);

private:

    void GenerateExtendedKey();

    std::vector<uint8_t> key_bytes_;
    std::vector<uint8_t> extended_key_bytes_;
};


#endif //APTOS_PRIVATEKEY_H
