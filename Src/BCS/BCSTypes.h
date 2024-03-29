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

#if defined(_WIN32) || defined(_WIN64)
    #if defined(BUILDING_APTOS_DLL)
        #define APTOS_API __declspec(dllexport)
    #else
        #define APTOS_API __declspec(dllimport)
    #endif
#else
    #define APTOS_API
#endif

namespace Aptos::BCS
{
    class Serialization;
    class Deserialization;

    // See type_tag.py
    enum class TypeTag
    {
        BOOL,            // int = 0
        U8,              // int = 1
        U64,             // int = 2
        U128,            // int = 3
        ACCOUNT_ADDRESS, // int = 4
        SIGNER,          // int = 5
        VECTOR,          // int = 6
        STRUCT,          // int = 7
        U16,
        U32,
        U256,
        SCRIPT,
        ENTRY_FUNCTION,
    };

    /// <summary>
    /// An interfaces that enforces types to implement a serialization method.
    /// </summary>
    class APTOS_API ISerializable
    {
    public:
        /// <summary>
        /// Serialize the object.
        /// </summary>
        /// <param name="serializer"></param>
        virtual void Serialize(Serialization &serializer) const = 0;

        /// <summary>
        /// Deserializes a byte array hosted inside the Deserializer.
        /// </summary>
        /// <param name="deserializer"></param>
        /// <returns></returns>
        static std::shared_ptr<ISerializable> Deserialize(Deserialization &deserializer);
        virtual std::size_t GetHashCode() const;
        ;
        virtual std::string ToString() const = 0;
    };

    class APTOS_API ISerializableTag : public ISerializable
    {
    public:
        /// <summary>
        /// Returns the type of type tag.
        /// </summary>
        /// <returns>A TypeTag enum.</returns>
        virtual TypeTag Variant() const = 0;

        /// <summary>
        /// Serializes the type tag using it's own serializaton method.
        /// </summary>
        /// <param name="serializer"></param>
        virtual void SerializeTag(Serialization &serializer)
        {
            this->Serialize(serializer);
        }

        /// <summary>
        /// Deserialize a tag based on it's type.
        /// </summary>
        /// <param name="deserializer"></param>
        /// <returns>An object.</returns>
        static std::shared_ptr<ISerializableTag> DeserializeTag(Deserialization &deserializer);

        std::string ToString() const override;
    };
}
#endif // APTOS_BCSTYPES_H
