//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_BCSTYPES_H
#define APTOS_BCSTYPES_H
#include <stdexcept>
#include <vector>
#include <string>
#include <cstdint>
#include <memory>

class Serialization;
class Deserialization;

enum class TypeTag {
    BOOL, // int = 0
    U8, // int = 1
    U64, // int = 2
    U128, // int = 3
    ACCOUNT_ADDRESS, // int = 4
    SIGNER, // int = 5
    VECTOR, // int = 6
    STRUCT, // int = 7
    U16,
    U32,
    U256,
    SCRIPT,
    ENTRY_FUNCTION
};

class ISerializable {
public:
    virtual void Serialize(Serialization& serializer) const = 0;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    virtual std::size_t GetHashCode();;
    virtual ~ISerializable() {}
    virtual std::string ToString() const = 0;
};

class ISerializableTag : public ISerializable {
public:
    virtual TypeTag Variant() const = 0;
    virtual void SerializeTag(Serialization& serializer) {
        this->Serialize(serializer);
    }
    static std::shared_ptr<ISerializableTag> DeserializeTag(Deserialization& deserializer);
    virtual ~ISerializableTag() {}

    std::string ToString() const override;
};

#endif //APTOS_BCSTYPES_H
