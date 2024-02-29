//
// Created by Anh NPH on 11/09/2023.
//

#include "Ed25519Signature.h"
#include <stdexcept>
#include <sstream>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include "../HDWallet/Utils/Utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
namespace Aptos::Accounts
{
    Ed25519Signature::Ed25519Signature(const CryptoPP::SecByteBlock &signature)
        : _signatureBytes(signature) {}

    bool Ed25519Signature::Equals(const Ed25519Signature &rhs) const
    {
        return _signatureBytes == rhs._signatureBytes;
    }

    CryptoPP::SecByteBlock Ed25519Signature::Data() const
    {
        return _signatureBytes;
    }

    void Ed25519Signature::Serialize(BCS::Serialization &serializer) const
    {
        auto bytes = Utils::SecBlockToByteVector(_signatureBytes);
        serializer.SerializeBytes(bytes);
    }

    std::shared_ptr<BCS::ISerializable> Ed25519Signature::Deserialize(BCS::Deserialization &deserializer)
    {
        CryptoPP::SecByteBlock sigBytes = Utils::ByteVectorToSecBlock(deserializer.ToBytes());
        if (sigBytes.size() != SignatureLength)
        {
            throw std::runtime_error("Length mismatch");
        }
        return std::make_shared<Ed25519Signature>(sigBytes);
    }

    bool Ed25519Signature::operator==(const Ed25519Signature &other) const
    {
        return ToString() == other.ToString();
    }

    std::string Ed25519Signature::ToString() const
    {
        std::ostringstream stream;
        for (auto byte : _signatureBytes)
        {
            stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }

        std::string _signature = "0x" + stream.str();
        return _signature;
    }
}