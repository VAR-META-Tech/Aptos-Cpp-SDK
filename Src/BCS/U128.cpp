#include "U128.h"
#include "Serialization.h"
#include "Deserialization.h"
#include <cryptopp/sha.h>     // for SHA256
#include <cryptopp/hex.h>     // for StringSource and HexEncoder
#include <cryptopp/filters.h> // for StringSink
#include <cryptopp/cryptlib.h>
#include <sstream>

using namespace CryptoPP;
namespace Aptos::BCS
{
    U128::U128(CryptoPP::Integer value): value(value)
    {
    }

    void U128::Serialize(Serialization &serializer) const
    {
        serializer.SerializeU128(value);
    }

    std::shared_ptr<U128> U128::Deserialize(Deserialization &deserializer)
    {
        CryptoPP::Integer value = deserializer.DeserializeU128();
        return std::make_shared<U128>(value);
    }

    std::string U128::ToString() const
    {
        std::ostringstream oss;
        oss << this->value;
        return oss.str();
    }

    bool U128::Equals(const U128 &other) const
    {
        return this->value == other.value;
    }

    size_t U128::GetHashCode() const
    {
        byte digest[CryptoPP::SHA256::DIGESTSIZE];
        std::string strValue = this->ToString();

        CryptoPP::SHA256().CalculateDigest(digest, (byte *)strValue.c_str(), strValue.length());

        // Convert the hash to a string
        CryptoPP::HexEncoder encoder;
        std::string output;
        encoder.Attach(new CryptoPP::StringSink(output));
        encoder.Put(digest, sizeof(digest));
        encoder.MessageEnd();

        // Use std::hash to convert the hash string to a size_t
        return std::hash<std::string>{}(output);
    }

    CryptoPP::Integer U128::Deserialize(const std::vector<uint8_t> &data)
    {
        if (data.size() < 16)
        {
            throw std::runtime_error("Not enough bytes to deserialize a CryptoPP::Integer");
        }
        // Convert bytes to CryptoPP::Integer
        CryptoPP::Integer res(data.data(), 16, CryptoPP::Integer::UNSIGNED, LITTLE_ENDIAN_ORDER);
        return res;
    }

    TypeTag U128::Variant() const
    {
        return TypeTag::U128;
    }

    CryptoPP::Integer U128::GetValue() const
    {
        return this->value;
    }
}