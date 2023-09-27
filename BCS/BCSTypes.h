//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_BCSTYPES_H
#define APTOS_BCSTYPES_H
#include <stdexcept>
#include <vector>
#include <string>

class Serialization;
class Deserialization;

enum class TypeTag{
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
    U256
};

class ISerializable {
public:
    // Pure virtual function providing interface framework.
    virtual void Serialize(Serialization& serializer) = 0;

    // Static function equivalent
    static ISerializable* Deserialize(Deserialization& deserializer) {
        // You can throw an exception here or handle it in your own way
        throw std::logic_error("The method or operation is not implemented.");
    }

    virtual std::size_t GetHashCode() {
        // Your hash logic here. This is just a simple example.
        std::size_t seed = 0;
        //boost::hash_combine(seed, int_member);
        //boost::hash_combine(seed, boost::hash_value(string_member));
        return seed;
    };

    // Virtual destructor to ensure correct deletion through base-class pointers
    virtual ~ISerializable() {}

    std::string ToString() {
        return std::string();
    }
};

class ISerializableTag : public ISerializable {
public:
    virtual TypeTag Variant() = 0;
    virtual void* GetValue() = 0;
    virtual void SerializeTag(Serialization& serializer) {
        this->Serialize(serializer);
    }
    static ISerializableTag* DeserializeTag(Deserialization& deserializer);

    std::string ToString();
};

#endif //APTOS_BCSTYPES_H
