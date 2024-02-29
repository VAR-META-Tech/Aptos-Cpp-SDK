#include "StructTag.h"

using namespace Aptos::Accounts;
namespace Aptos::BCS
{
    StructTag::StructTag(AccountAddress address, std::string moduleName, std::string name,
                         const std::vector<std::shared_ptr<ISerializableTag>> &typeArgs)
        : address(address), moduleName(moduleName), name(name), typeArgs(typeArgs) {}

    TypeTag StructTag::Variant() const { return TypeTag::STRUCT; }

    void StructTag::Serialize(Serialization &serializer) const
    {
        serializer.SerializeU32AsUleb128(static_cast<uint32_t>(Variant()));
        address.Serialize(serializer);
        serializer.Serialize(moduleName);
        serializer.Serialize(name);
        serializer.SerializeU32AsUleb128(static_cast<uint32_t>(typeArgs.size()));

        for (auto typeArg : typeArgs)
        {
            typeArg->Serialize(serializer);
        }
    }

    std::shared_ptr<StructTag> StructTag::Deserialize(Deserialization &deserializer)
    {
        auto address = std::dynamic_pointer_cast<AccountAddress>(AccountAddress::Deserialize(deserializer));
        std::string moduleName = deserializer.DeserializeString();
        std::string name = deserializer.DeserializeString();

        uint32_t length = deserializer.DeserializeUleb128();
        std::vector<std::shared_ptr<ISerializableTag>> typeArgsList;

        while (typeArgsList.size() < length)
        {
            std::shared_ptr<ISerializableTag> val = ISerializableTag::DeserializeTag(deserializer);
            typeArgsList.push_back(val);
        }

        return std::make_shared<StructTag>(*address, moduleName, name, typeArgsList);
    }

    bool StructTag::Equals(const StructTag &other) const
    {
        return (address == other.address &&
                moduleName == other.moduleName &&
                name == other.name &&
                typeArgs == other.typeArgs);
    }

    std::string StructTag::ToString() const
    {
        std::string value = address.ToString() + "::" + moduleName + "::" + name;

        if (!typeArgs.empty())
        {
            value += "<" + typeArgs[0]->ToString();
            for (size_t i = 1; i < typeArgs.size(); ++i)
            {
                value += ", " + typeArgs[i]->ToString();
            }
            value += ">";
        }

        return value;
    }

    StructTag StructTag::FromStr(const std::string &typeTag)
    {
        std::string name;
        int index = 0;
        while (index < typeTag.length())
        {
            char letter = typeTag[index];
            index += 1;

            if (letter == '<')
            {
                throw std::runtime_error("Parsing type arguments is not implemented yet");
            }
            else
            {
                name += letter;
            }
        }

        // Split the name into parts using "::" as the delimiter
        size_t pos = 0;
        std::vector<std::string> split;
        while ((pos = name.find("::")) != std::string::npos)
        {
            split.push_back(name.substr(0, pos));
            name.erase(0, pos + 2); // +2 to skip the "::" itself
        }
        split.push_back(name); // Add the final part

        if (split.size() < 3)
        {
            throw std::runtime_error("Invalid typeTag format");
        }

        return StructTag(AccountAddress::FromHex(split[0]), split[1], split[2], {});
    }

    bool operator==(const StructTag &lhs, const StructTag &rhs)
    {
        return lhs.Equals(rhs);
    }
}