//
// Created by Anh NPH on 22/09/2023.
//

#ifndef APTOS_BSTRING_H
#define APTOS_BSTRING_H
#include "BCSTypes.h"

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a string in BCS.
    /// </summary>
    class APTOS_API BString : public ISerializable
    {
    private:
        /// <summary>
        /// The internal string value.
        /// </summary>
        std::string value;

    public:
        BString();

        /// <summary>
        /// Creates a BString from a string.
        /// </summary>
        /// <param name="value">A string value.</param>
        explicit BString(const std::string &value);

        /// <summary>
        /// Serializes the BString object using the given Serializer.
        /// </summary>
        /// <param name="serializer">The Serializer object.</param>
        void Serialize(Serialization &serializer) const override;

        /// <summary>
        /// Deserializes a give byte array into a UTF8 compliant string.
        /// </summary>
        /// <param name="data">A string represented as a byte array.</param>
        /// <returns></returns>
        static std::string Deserialize(const std::vector<uint8_t> &data);

        /// <summary>
        /// Deserializes a byte array contained by the Deserializer.
        /// </summary>
        /// <param name="deserializer">The Deserializer that contains the bytes.</param>
        /// <returns>A BString object.</returns>
        static std::shared_ptr<BString> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        bool Equals(const BString &other) const;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <inheritdoc/>
        size_t GetHashCode() const override;

        /// <inheritdoc/>
        std::string GetValue() const;

        /// <summary>
        /// Utility function used to RemoveBOM prefixes.
        /// </summary>
        /// <param name="data">A string represented as a byte array.</param>
        /// <returns>The cleaned byte array.</returns>
        std::vector<uint8_t> RemoveBOM(const std::vector<uint8_t> &data);

        bool operator<(const BString &other) const;

        bool operator==(const BString &other) const;
    };
}
#endif // APTOS_BSTRING_H
