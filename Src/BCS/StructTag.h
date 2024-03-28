#ifndef STRUCTTAG_H
#define STRUCTTAG_H
#include <string>
#include <vector>
#include "BCSTypes.h"
#include "../Accounts/AccountAddress.h"

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a struct tag.
    /// </summary>
    class APTOS_API StructTag : public ISerializableTag
    {
    public:
        /// <summary>
        /// Creates a StructTag object from an address, module, function name,
        /// and type arguments.
        /// </summary>
        /// <param name="address">An AccountAddress.</param>
        /// <param name="module">The module name.</param>
        /// <param name="name">The function name.</param>
        /// <param name="typeArgs">A list of type arguments.</param>
        explicit StructTag(Accounts::AccountAddress address, std::string moduleName, std::string name,
                  const std::vector<std::shared_ptr<ISerializableTag>> &typeArgs);

        /// <inheritdoc/>
        TypeTag Variant() const override;

        /// <inheritdoc/>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        static std::shared_ptr<StructTag> Deserialize(Deserialization &deserializer);

        /// <inheritdoc/>
        bool Equals(const StructTag &other) const;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <inheritdoc/>
        static StructTag FromStr(const std::string &typeTag);

    private:
        /// <summary>
        /// The account address of the struct tag.
        /// </summary>
        Accounts::AccountAddress address;

        /// <summary>
        /// The module name of the struct tag.
        /// </summary>
        std::string moduleName;

        /// <summary>
        /// The function name of the struct tag.
        /// </summary
        std::string name;

        /// <summary>
        /// A set of type arguments, if any.
        /// </summary>
        std::vector<std::shared_ptr<ISerializableTag>> typeArgs;
    };

    bool APTOS_API operator==(const StructTag &lhs, const StructTag &rhs);
}
#endif // STRUCTTAG_H
