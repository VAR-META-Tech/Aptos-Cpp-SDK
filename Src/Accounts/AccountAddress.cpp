//
// Created by Anh NPH on 28/09/2023.
//

#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include "AccountAddress.h"
#include "../HDWallet/Utils/Utils.h"
#include <cryptopp/sha3.h>
#include <sstream>
#include <iomanip>
namespace Aptos::Accounts
{
    CryptoPP::SecByteBlock AccountAddress::addressBytes() const
    {
        return _addressBytes;
    }

    AccountAddress::AccountAddress(const CryptoPP::SecByteBlock &addressBytes)
    {
        if (addressBytes.size() != Length)
        {
            throw std::invalid_argument("Invalid address length.");
        }
        _addressBytes = addressBytes;
    }
    
    std::string AccountAddress::ToString() const
    {
        std::ostringstream addressHexStream;
        for (const auto &b : _addressBytes)
        {
            addressHexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b);
        }

        std::string addressHex = addressHexStream.str();

        if (IsSpecial())
        {
            addressHex.erase(0, addressHex.find_first_not_of('0'));
            if (addressHex.empty())
            {
                addressHex = "0";
            }
        }

        return "0x" + addressHex;
    }

    bool AccountAddress::IsSpecial() const
    {
        bool allZeros = std::all_of(_addressBytes.begin(), _addressBytes.end() - 1, [](int i)
                                    { return i == 0; });
        return allZeros && (_addressBytes[_addressBytes.size() - 1] < 0b10000);
    }

    AccountAddress AccountAddress::FromHex(std::string address)
    {
        if (address.empty())
        {
            throw std::invalid_argument("Address string is empty.");
        }

        std::string addr = address;
        if (addr.length() == 1)
        {
            addr = "0x" + addr;
        }

        if (addr.starts_with("0x"))
        {
            addr = addr.substr(2);
        }
        size_t expectedLength = AccountAddress::Length * 2;

        if (addr.length() < expectedLength)
        {
            std::string pad(expectedLength - addr.length(), '0');
            addr = pad + addr;
        }
        std::vector<uint8_t> byteArray;
        for (size_t i = 0; i < addr.length(); i += 2)
        {
            std::string byteStr = addr.substr(i, 2);
            auto byte = static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16));
            byteArray.push_back(byte);
        }

        return AccountAddress(Utils::ByteVectorToSecBlock(byteArray));
    }

    AccountAddress AccountAddress::FromKey(const CryptoPP::SecByteBlock &publicKey)
    {
        CryptoPP::SecByteBlock addressBytes{Utils::Ed25519PublicKeySizeInBytes + 1}; // +1 to contain signature scheme byte
        std::copy_n(publicKey.begin(), Utils::Ed25519PublicKeySizeInBytes, addressBytes.begin());
        CryptoPP::byte sigScheme = 0x00;                                   // signature scheme byte
        addressBytes[Utils::Ed25519PublicKeySizeInBytes] = sigScheme;      // Append signature scheme byte to the end
        CryptoPP::SecByteBlock result{Utils::Ed25519PublicKeySizeInBytes}; // Result hash must be 32 bytes
        CryptoPP::SHA3_256 sha3;
        sha3.Update(addressBytes.data(), addressBytes.size());
        sha3.Final(result);
        return AccountAddress(result);
    }

    AccountAddress AccountAddress::FromKey(const PublicKey &publicKey)
    {
        return AccountAddress::FromKey(publicKey.KeyBytes());
    }

    void AccountAddress::Serialize(BCS::Serialization &serializer) const
    {
        serializer.SerializeFixedBytes(Utils::SecBlockToByteVector(_addressBytes));
    }

    size_t AccountAddress::GetHashCode() const
    {
        return ISerializable::GetHashCode();
    }

    BCS::TypeTag AccountAddress::Variant() const
    {
        return BCS::TypeTag::ACCOUNT_ADDRESS;
    }

    std::shared_ptr<BCS::ISerializableTag> AccountAddress::Deserialize(BCS::Deserialization &deserializer)
    {
        CryptoPP::SecByteBlock addressBytes = Utils::ByteVectorToSecBlock(deserializer.FixedBytes(AccountAddress::Length));
        return std::make_shared<AccountAddress>(addressBytes);
    }

    bool operator==(const AccountAddress &lhs, const AccountAddress &rhs)
    {
        return lhs.ToString() == rhs.ToString();
    }

    AccountAddress AccountAddress::ForNamedObject(const AccountAddress &creator, const CryptoPP::SecByteBlock &seed)
    {
        // Create a SHA3-256 hash object
        CryptoPP::SHA3_256 sha3;

        // Concatenate the creator's address, seed, and AuthKeyScheme value
        CryptoPP::SecByteBlock data;
        data.Append(creator.addressBytes());
        data.Append(seed);
        data.Append(1, AuthKeyScheme::DeriveObjectAddressFromSeed);

        // Calculate the hash
        CryptoPP::SecByteBlock result(Utils::Ed25519PublicKeySizeInBytes);
        sha3.Update(data.data(), data.size());
        sha3.Final(result);

        return AccountAddress(result);
    }

    AccountAddress AccountAddress::ForNamedToken(const AccountAddress &creator, const std::string &collectionName, const std::string &tokenName)
    {
        std::string combined = collectionName + "::" + tokenName;
        return AccountAddress::ForNamedObject(creator, Utils::StringToSecByteBlock(combined));
    }

    AccountAddress AccountAddress::ForNamedCollection(AccountAddress creator, std::string collectionName)
    {
        return AccountAddress::ForNamedObject(creator, Utils::StringToSecByteBlock(collectionName));
    }

    AccountAddress AccountAddress::ForResourceAccount(AccountAddress creator, const CryptoPP::SecByteBlock &seed)
    {
        CryptoPP::SHA3_256 sha3;

        CryptoPP::SecByteBlock data;
        data.Append(creator.addressBytes());
        data.Append(seed);
        data.Append(1, AuthKeyScheme::DeriveResourceAccountAddress);

        // Calculate the hash
        CryptoPP::SecByteBlock result(Utils::Ed25519PublicKeySizeInBytes);
        sha3.Update(data.data(), data.size());
        sha3.Final(result);

        return AccountAddress(result);
    }

    AccountAddress AccountAddress::FromMultiEd25519(MultiPublicKey keys)
    {
        CryptoPP::SHA3_256 sha3;
        CryptoPP::SecByteBlock result(Utils::Ed25519PublicKeySizeInBytes);

        std::vector<uint8_t> keyBytes = keys.ToBytes();

        // Calculate the hash
        CryptoPP::SecByteBlock data;
        data.Append(Utils::ByteVectorToSecBlock(keyBytes));
        data.Append(1, AuthKeyScheme::MultiEd25519);

        sha3.Update(data.data(), data.size());
        sha3.Final(result);
        return AccountAddress(result);
    }
}
