//
// Created by Anh NPH on 11/09/2023.
//

#include "Signature.h"
#include <stdexcept>
#include <sstream>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

Signature::Signature(const std::vector<CryptoPP::byte>& signature)
        : _signatureBytes(signature) {}

std::vector<CryptoPP::byte> Signature::Data() const {
    return _signatureBytes;
}

void Signature::Serialize(Serialization& serializer) {
    serializer.SerializeBytes(_signatureBytes);
}

Signature Signature::Deserialize(Deserialization& deserializer) {
    std::vector<CryptoPP::byte> sigBytes = deserializer.ToBytes();
    if (sigBytes.size() != SignatureLength)
        throw std::runtime_error("Length mismatch");

    return Signature(sigBytes);
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