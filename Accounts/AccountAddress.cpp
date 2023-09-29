//
// Created by Anh NPH on 28/09/2023.
//

#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include "AccountAddress.h"

AccountAddress::AccountAddress(std::vector<CryptoPP::byte> &addressBytes) {
    if (_addressBytes.size() != Length){
        throw std::invalid_argument("Invalid address length.");
    }
    _addressBytes = addressBytes;
}

std::string AccountAddress::ToString() {
    std::string addressHex;
    CryptoPP::StringSource ss(_addressBytes.data(), _addressBytes.size(), true,
                              new CryptoPP::HexEncoder(
                                      new CryptoPP::StringSink(addressHex),
                                      false
                              ) // HexDecoder
    );
    return "0x" + addressHex;
}

bool AccountAddress::IsSpecial() {
    bool allZeros = std::all_of(_addressBytes.begin(), _addressBytes.end() - 1, [](int i){ return i == 0; });
    return allZeros && (_addressBytes[_addressBytes.size() - 1] < 0b10000);
}

AccountAddress AccountAddress::FromHex(std::string address) {
    if (address.substr(0, 2) == "0x") {
        address = address.substr(2);
    }
    CryptoPP::HexDecoder decoder;
    decoder.Put((CryptoPP::byte*)address.data(),address.size());
    decoder.MessageEnd();
    size_t size = decoder.MaxRetrievable();
    std::vector<CryptoPP::byte> addressBytes;
    if(size && size <= SIZE_MAX)
    {
        addressBytes.resize(size);
        decoder.Get((CryptoPP::byte*)&addressBytes[0], addressBytes.size());
    }
    return AccountAddress(addressBytes);
}

AccountAddress AccountAddress::FromKey(std::vector<CryptoPP::byte> &publicKey) {
    CryptoPP::SHA256 hash;
    hash.Update(publicKey.data(), publicKey.size());
    std::vector<CryptoPP::byte> addressBytes;
    addressBytes.resize(publicKey.size()+1);
    hash.Final(addressBytes.data());
    return AccountAddress(addressBytes);
}

void AccountAddress::Serialize(Serialization &serializer) {
    serializer.SerializeFixedBytes(_addressBytes);
}

size_t AccountAddress::GetHashCode() {
    return ISerializable::GetHashCode();
}

TypeTag AccountAddress::Variant() {
    return TypeTag::ACCOUNT_ADDRESS;
}

AccountAddress AccountAddress::Deserialize(Deserialization& deserializer) {
    std::vector<CryptoPP::byte> addressBytes = deserializer.FixedBytes(AccountAddress::Length);
    return AccountAddress(addressBytes);
}

void *AccountAddress::GetValue() {
    return nullptr;
}
