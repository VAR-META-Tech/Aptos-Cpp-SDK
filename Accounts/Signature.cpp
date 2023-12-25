//
// Created by Anh NPH on 11/09/2023.
//

#include "Signature.h"
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
    Signature::Signature(const CryptoPP::SecByteBlock &signature)
        : _signatureBytes(signature) {}

    bool Signature::Equals(const Signature &rhs) const
    {
        return _signatureBytes == rhs._signatureBytes;
    }

    CryptoPP::SecByteBlock Signature::Data() const
    {
        return _signatureBytes;
    }

    void Signature::Serialize(BCS::Serialization &serializer) const
    {
        auto bytes = Utils::SecBlockToByteVector(_signatureBytes);
        serializer.SerializeBytes(bytes);
    }

    std::shared_ptr<BCS::ISerializable> Signature::Deserialize(BCS::Deserialization &deserializer)
    {
        CryptoPP::SecByteBlock sigBytes = Utils::ByteVectorToSecBlock(deserializer.ToBytes());
        if (sigBytes.size() != SignatureLength)
        {
            throw std::runtime_error("Length mismatch");
        }
        return std::make_shared<Signature>(sigBytes);
    }

    bool Signature::operator==(const Signature &other) const
    {
        return ToString() == other.ToString();
    }

    bool Signature::operator!=(const Signature &other) const
    {
        return !(*this == other);
    }

    std::string Signature::ToString() const
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