//
// Created by Anh NPH on 11/09/2023.
//

#include "Signature.h"
#include <stdexcept>
#include <sstream>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include "../HDWallet/Utils/Utils.h"

Signature::Signature(const CryptoPP::SecByteBlock& signature)
        : _signatureBytes(signature) {}

bool Signature::Equals(const Signature &rhs) const {
    return _signatureBytes == rhs._signatureBytes;
}

CryptoPP::SecByteBlock Signature::Data() const {
    return _signatureBytes;
}

void Signature::Serialize(Serialization& serializer) const {
    auto bytes = Utils::SecBlockToByteVector(_signatureBytes);
    serializer.SerializeBytes(bytes);
}

std::shared_ptr<ISerializable> Signature::Deserialize(Deserialization& deserializer) {
    CryptoPP::SecByteBlock sigBytes = Utils::ByteVectorToSecBlock(deserializer.ToBytes());
    if (sigBytes.size() != SignatureLength) {
        throw std::runtime_error("Length mismatch");
    }
    return std::make_shared<Signature>(sigBytes);
}

bool Signature::operator==(const Signature& other) const {
    return ToString() == other.ToString();
}

bool Signature::operator!=(const Signature& other) const {
    return !(*this == other);
}

std::string Signature::ToString() const {
    if (_signature.empty()) {
        CryptoPP::HexEncoder encoder;
        std::string hex;
        encoder.Attach(new CryptoPP::StringSink(hex));
        encoder.Put(_signatureBytes.data(), _signatureBytes.size());
        encoder.MessageEnd();
        _signature = "0x" + hex;
    }
    return _signature;
}
