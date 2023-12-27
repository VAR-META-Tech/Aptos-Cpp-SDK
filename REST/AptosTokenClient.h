#ifndef APTOSTOKENCLIENT_H
#define APTOSTOKENCLIENT_H
#include <string>
#include <memory>
#include "Model/Resources/Base/ResourceDataBase.h"
#include "../Accounts/AccountAddress.h"
#include "../BCS/BString.h"
#include "../BCS/Bytes.h"

namespace Aptos::Rest
{
    /// <summary>
    /// Interface implemented by all resource types.
    /// Necessary to create Dictionaries or Lists of different resources.
    /// </summary>
    class IResource
    {
    public:
        virtual std::string GetStructTag() const = 0;
        static std::shared_ptr<IResource> Parse(std::shared_ptr<AptosRESTModel::ResourceDataBase> resource);
        virtual std::string ToString() const = 0;
    };

    /// <summary>
    /// Represents an Object resource.
    /// </summary>
    class Object : public IResource
    {
    private:
        bool AllowUngatedTransfer;
        Accounts::AccountAddress Owner;

    public:
        static const std::string StructTag;
        Object(bool allowUngatedTransfer, Accounts::AccountAddress owner);

        /// <summary>
        /// Parses a JSON string representation of a resource into an Object.
        /// </summary>
        /// <param name="resourceJson"></param>
        /// <returns></returns>
        static std::shared_ptr<IResource> Parse(std::shared_ptr<AptosRESTModel::ResourceDataBase> resource);
        std::string GetStructTag() const override;
        std::string ToString() const override;
    };

    /// <summary>
    /// Represents a Collection resource.
    /// </summary>
    class Collection : public IResource
    {
    private:
        Accounts::AccountAddress Creator;
        std::string Description;
        std::string Name;
        std::string Uri;

    public:
        static const std::string StructTag;

        Collection(Accounts::AccountAddress creator, const std::string &description, const std::string &name, const std::string &uri);

        static std::shared_ptr<IResource> Parse(std::shared_ptr<AptosRESTModel::ResourceDataBase> resource);

        std::string GetStructTag() const override;
        std::string ToString() const override;
    };

    class Royalty : public IResource
    {
    private:
        int Numerator;
        int Denominator;
        Accounts::AccountAddress PayeeAddress;

    public:
        static const std::string StructTag;

        Royalty(int numerator, int denominator, const Accounts::AccountAddress &payeeAddress);
        static std::shared_ptr<IResource> Parse(std::shared_ptr<AptosRESTModel::ResourceDataBase> resource);
        std::string GetStructTag() const override;
        std::string ToString() const override;
    };

    /// <summary>
    /// Represent a Token resource.
    /// </summary>
    class Token : public IResource
    {
    private:
        Accounts::AccountAddress Collection;
        int Index;
        std::string Description;
        std::string Name;
        std::string Uri;

    public:
        static const std::string StructTag;
        Token(const Accounts::AccountAddress &collection, int index, const std::string &description, const std::string &name, const std::string &uri);
        static std::shared_ptr<IResource> Parse(std::shared_ptr<AptosRESTModel::ResourceDataBase> resource);
        std::string GetStructTag() const override;
        std::string ToString() const override;
    };

    class Property
    {
    private:
        BCS::BString Name;
        BCS::BString PropertyType;
        std::shared_ptr<BCS::ISerializable> Value;

    public:
        static const int BOOLTYPE = 0;
        static const int U8TYPE = 1;
        static const int U16TYPE = 2;
        static const int U32TYPE = 3;
        static const int U64TYPE = 4;
        static const int U128TYPE = 5;
        static const int U256TYPE = 6;
        static const int ADDRESSTYPE = 7;
        static const int BYTE_VECTORTYPE = 8;
        static const int STRINGTYPE = 9;

        Property(const std::string &name, const std::string &propertyType, std::shared_ptr<BCS::ISerializable> value);
        Property(const BCS::BString &name, const BCS::BString &propertyType, std::shared_ptr<BCS::ISerializable> value);
        std::string ToString() const;
        std::vector<uint8_t> SerializeValue() const;
        std::vector<std::shared_ptr<BCS::ISerializable>> ToTransactionArguments();
        static Property Parse(const std::string &Name, int PropertyType, const BCS::Bytes &Value);
        static Property BoolProp(const std::string &Name, bool Value);
        static Property U8Prop(const std::string &Name, uint8_t Value);
        static Property U32Prop(const std::string &Name, uint32_t Value);
        static Property U64Prop(const std::string &Name, uint64_t Value);
        //    static Property U128Prop(const std::string& Name, std::uint128_t Value);
        static Property StringProp(const std::string &Name, const std::string &Value);
        static Property BytesProp(const std::string &Name, const std::vector<uint8_t> &Value);
        const BCS::BString &getName() const;
        const BCS::BString &getPropertyType() const;
        const std::shared_ptr<BCS::ISerializable> &getValue() const;
    };

    class PropertyMap : public IResource
    {
    private:
        std::vector<Property> Properties;

    public:
        static const std::string StructTag;
        explicit PropertyMap(const std::vector<Property> &Properties);
        std::string ToString() const override;
        std::tuple<std::vector<BCS::BString>, std::vector<BCS::BString>, std::vector<std::vector<uint8_t>>> ToTuple() const;
        static std::shared_ptr<IResource> Parse(std::shared_ptr<AptosRESTModel::ResourceDataBase> resource);
        std::string GetStructTag() const override;
    };

    class ReadObject
    {
    public:
        using FuncValue = std::shared_ptr<IResource>(std::shared_ptr<AptosRESTModel::ResourceDataBase>);
        static std::map<std::string, FuncValue *> ResourceMap;
        explicit ReadObject(const std::unordered_map<std::string, std::shared_ptr<IResource>> &Resources);
        std::string ToString() const;

    private:
        std::unordered_map<std::string, std::shared_ptr<IResource>> Resources;
    };
}
#endif // APTOSTOKENCLIENT_H
