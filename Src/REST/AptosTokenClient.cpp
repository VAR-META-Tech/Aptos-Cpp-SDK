#include "AptosTokenClient.h"
#include <stdexcept>
#include "Constant.h"
#include "Model/Resources/ObjectResource.h"
#include "Model/Resources/CollectionResource.h"
#include "Model/Resources/RoyaltyResource.h"
#include "Model/Resources/TokenResource.h"
#include "Model/Resources/PropertyMapResource.h"
#include "../BCS/BCS.h"
#include "../Accounts/AccountAddress.h"
#include "../HDWallet/Utils/Utils.h"
#include "Model/Resources/Base/ResourceBaseListConverter.h"
#include "../BCS/StructTag.h"

using namespace Aptos::Accounts;
using namespace Aptos::BCS;
namespace Aptos::Rest
{
    const std::string Object::StructTag = "0x1::object::ObjectCore";
    const std::string Collection::StructTag = "0x4::collection::Collection";
    const std::string Royalty::StructTag = "0x4::royalty::Royalty";
    const std::string Token::StructTag = "0x4::token::Token";
    const std::string PropertyMap::StructTag = "0x4::property_map::PropertyMap";

    std::map<std::string, ReadObject::FuncValue *> ReadObject::ResourceMap = {
        {Collection::StructTag, &Collection::Parse},
        {Object::StructTag, &Object::Parse},
        {PropertyMap::StructTag, &PropertyMap::Parse},
        {Royalty::StructTag, &Royalty::Parse},
        {Token::StructTag, &Token::Parse}};

    std::shared_ptr<IResource> IResource::Parse(std::shared_ptr<AptosRESTModel::ResourceDataBase> resource)
    {
        throw std::runtime_error("Not implemented");
    }

    Object::Object(bool allowUngatedTransfer, AccountAddress owner) : AllowUngatedTransfer(allowUngatedTransfer), Owner(owner) {}

    std::shared_ptr<IResource> Object::Parse(std::shared_ptr<AptosRESTModel::ResourceDataBase> resource)
    {
        auto objResData = std::dynamic_pointer_cast<AptosRESTModel::ObjectResourceData>(resource);

        if (objResData != nullptr)
        {
            bool allowUngatedTransfer = objResData->getAllowUngatedTransfer();
            AccountAddress owner = AccountAddress::FromHex(objResData->getOwner());
            return std::make_shared<Object>(allowUngatedTransfer, owner);
        }

        throw std::runtime_error("Invalid resource data type");
    }

    std::string Object::GetStructTag() const
    {
        return StructTag;
    }

    std::string Object::ToString() const
    {
        return "Object[allow_ungated_transfer: " + std::to_string(AllowUngatedTransfer) + ", owner: " + Owner.ToString() + "]";
    }

    Collection::Collection(AccountAddress creator, const std::string &description, const std::string &name, const std::string &uri)
        : Creator(creator), Description(description), Name(name), Uri(uri) {}

    std::shared_ptr<IResource> Collection::Parse(std::shared_ptr<AptosRESTModel::ResourceDataBase> resource)
    {
        using namespace AptosRESTModel;
        std::shared_ptr<CollectionResourceData> collectionData = std::dynamic_pointer_cast<CollectionResourceData>(resource);

        if (collectionData != nullptr)
        {
            AccountAddress creator = AccountAddress::FromHex(collectionData->getCreator());
            std::string description = collectionData->getDescription();
            std::string name = collectionData->getName();
            std::string uri = collectionData->getUri();
            return std::make_shared<Collection>(creator, description, name, uri);
        }

        throw std::runtime_error("Invalid resource data type");
    }

    std::string Collection::GetStructTag() const
    {
        return StructTag;
    }

    std::string Collection::ToString() const
    {
        return "AccountAddress[creator: " + Creator.ToString() + ", description: " + Description + ", name: " + Name + ", uri: " + Uri + "]";
    }

    Royalty::Royalty(int numerator, int denominator, const AccountAddress &payeeAddress)
        : Numerator(numerator), Denominator(denominator), PayeeAddress(payeeAddress) {}

    std::shared_ptr<IResource> Royalty::Parse(std::shared_ptr<AptosRESTModel::ResourceDataBase> resource)
    {
        using namespace AptosRESTModel;
        std::shared_ptr<RoyaltyResourceData> royaltyResData = std::dynamic_pointer_cast<RoyaltyResourceData>(resource);

        if (royaltyResData != nullptr)
        {
            int numerator = std::stoi(royaltyResData->getNumerator());
            int denominator = std::stoi(royaltyResData->getDenominator());
            AccountAddress payeeAddress = AccountAddress::FromHex(royaltyResData->getPayeeAddress());

            return std::make_shared<Royalty>(numerator, denominator, payeeAddress);
        }

        throw std::runtime_error("Invalid resource data type");
    }

    std::string Royalty::GetStructTag() const
    {
        return StructTag;
    }

    std::string Royalty::ToString() const
    {
        return "Royalty[numerator: " + std::to_string(Numerator) + ", denominator: " + std::to_string(Denominator) + ", payee_address: " + PayeeAddress.ToString() + "]";
    }

    Token::Token(const AccountAddress &collection, int index, const std::string &description, const std::string &name, const std::string &uri)
        : Collection(collection), Index(index), Description(description), Name(name), Uri(uri) {}

    std::shared_ptr<IResource> Token::Parse(std::shared_ptr<AptosRESTModel::ResourceDataBase> resource)
    {
        using namespace AptosRESTModel;
        std::shared_ptr<TokenResourceData> tokenResData = std::dynamic_pointer_cast<TokenResourceData>(resource);

        if (tokenResData != nullptr)
        {
            AccountAddress collection = AccountAddress::FromHex(tokenResData->getCollectionProp().getInner());
            int index = std::stoi(tokenResData->getIndex());
            std::string description = tokenResData->getDescription();
            std::string name = tokenResData->getName();
            std::string uri = tokenResData->getUri();
            return std::make_shared<Token>(collection, index, description, name, uri);
        }

        throw std::runtime_error("Invalid resource data type");
    }

    std::string Token::GetStructTag() const
    {
        return StructTag;
    }

    std::string Token::ToString() const
    {
        return "Token[collection: " + Collection.ToString() + ", index: " + std::to_string(Index) + ", description: " + Description + ", name: " + Name + ", uri: " + Uri + "]";
    }

    const BString &Property::getName() const
    {
        return Name;
    }

    const BString &Property::getPropertyType() const
    {
        return PropertyType;
    }

    const std::shared_ptr<ISerializable> &Property::getValue() const
    {
        return Value;
    }

    Property::Property(const std::string &name, const std::string &propertyType, std::shared_ptr<ISerializable> value)
        : Name(name), PropertyType(propertyType), Value(value) {}

    Property::Property(const BString &name, const BString &propertyType, std::shared_ptr<ISerializable> value)
        : Name(name), PropertyType(propertyType), Value(value) {}

    std::string Property::ToString() const
    {
        return "Property[" + Name.ToString() + ", " + PropertyType.ToString() + ", " + Value->ToString();
    }

    std::vector<uint8_t> Property::SerializeValue() const
    {
        Serialization ser;
        if (PropertyType == BString("bool"))
        {
            auto boolValue = std::dynamic_pointer_cast<Bool>(Value);
            if (boolValue != nullptr)
            {
                boolValue->Serialize(ser);
            }
        }
        else if (PropertyType == BString("u8"))
        {
            auto u8Value = std::dynamic_pointer_cast<U8>(Value);
            if (u8Value != nullptr)
            {
                u8Value->Serialize(ser);
            }
        }
        else if (PropertyType == BString("u16"))
        {
            auto u16Value = std::dynamic_pointer_cast<U16>(Value);
            if (u16Value != nullptr)
            {
                u16Value->Serialize(ser);
            }
        }
        else if (PropertyType == BString("u32"))
        {
            auto u32Value = std::dynamic_pointer_cast<U32>(Value);
            if (u32Value != nullptr)
            {
                u32Value->Serialize(ser);
            }
        }
        else if (PropertyType == BString("u64"))
        {
            auto u64Value = std::dynamic_pointer_cast<U64>(Value);
            if (u64Value != nullptr)
            {
                u64Value->Serialize(ser);
            }
        }
        else if (PropertyType == BString("u128"))
        {
            auto u128Value = std::dynamic_pointer_cast<U128>(Value);
            if (u128Value != nullptr)
            {
                u128Value->Serialize(ser);
            }
        }
        else if (PropertyType == BString("u256"))
        {
            auto u256Value = std::dynamic_pointer_cast<U256>(Value);
            if (u256Value != nullptr)
            {
                u256Value->Serialize(ser);
            }
        }
        else if (PropertyType == BString("address"))
        {
            auto accountAddress = std::dynamic_pointer_cast<AccountAddress>(Value);
            if (accountAddress != nullptr)
            {
                accountAddress->Serialize(ser);
            }
        }
        else if (PropertyType == BString("0x1::string::String"))
        {
            auto bStringValue = std::dynamic_pointer_cast<BString>(Value);
            if (bStringValue != nullptr)
            {
                bStringValue->Serialize(ser);
            }
        }
        else if (PropertyType == BString("vector<u8>"))
        {
            auto sequenceVectorValue = std::dynamic_pointer_cast<Bytes>(Value);
            if (sequenceVectorValue != nullptr)
            {
                sequenceVectorValue->Serialize(ser);
            }
        }
        else
        {
            throw std::runtime_error("Invalid property type");
        }

        return ser.GetBytes();
    }

    std::vector<std::shared_ptr<ISerializable>> Property::ToTransactionArguments()
    {
        std::vector<std::shared_ptr<ISerializable>> args;
        args.push_back(std::make_shared<BString>(Name));
        args.push_back(std::make_shared<BString>(PropertyType));
        std::vector<uint8_t> serializedValue = SerializeValue();
        args.push_back(std::make_shared<Bytes>(serializedValue));

        return args;
    }

    Property Property::Parse(const std::string &Name, int PropertyType, const Bytes &Value)
    {
        Deserialization deser(Value.getValue());

        if (PropertyType == BOOLTYPE)
        {
            return Property(Name, "bool", std::make_shared<Bool>(deser.DeserializeBool()));
        }
        else if (PropertyType == U8TYPE)
        {
            return Property(Name, "u8", std::make_shared<U8>(deser.DeserializeU8()));
        }
        else if (PropertyType == U16TYPE)
        {
            return Property(Name, "u16", std::make_shared<U16>(deser.DeserializeU16()));
        }
        else if (PropertyType == U32TYPE)
        {
            return Property(Name, "u32", std::make_shared<U32>(deser.DeserializeU32()));
        }
        else if (PropertyType == U64TYPE)
        {
            return Property(Name, "u64", std::make_shared<U64>(deser.DeserializeU64()));
        }
        else if (PropertyType == U128TYPE)
        {
            return Property(Name, "u128", std::make_shared<U128>(deser.DeserializeU128()));
        }
        else if (PropertyType == U256TYPE)
        {
            return Property(Name, "u256", std::make_shared<U256>(deser.DeserializeU256()));
        }
        else if (PropertyType == ADDRESSTYPE)
        {
            return Property(Name, "address", AccountAddress::Deserialize(deser));
        }
        else if (PropertyType == STRINGTYPE)
        {
            return Property(Name, "0x1::string::String", std::make_shared<BString>(deser.DeserializeString()));
        }
        else if (PropertyType == BYTE_VECTORTYPE)
        {
            return Property(Name, "vector<u8>", std::make_shared<Bytes>(deser.ToBytes()));
        }
        else
        {
            throw std::runtime_error("Invalid property type");
        }
    }

    Property Property::BoolProp(const std::string &Name, bool Value)
    {
        return Property(Name, "bool", std::make_shared<Bool>(Value));
    }

    Property Property::U8Prop(const std::string &Name, uint8_t Value)
    {
        return Property(Name, "u8", std::make_shared<U8>(Value));
    }

    Property Property::U32Prop(const std::string &Name, uint32_t Value)
    {
        return Property(Name, "u32", std::make_shared<U32>(Value));
    }

    Property Property::U64Prop(const std::string &Name, uint64_t Value)
    {
        return Property(Name, "u64", std::make_shared<U64>(Value));
    }

    // Property Property::U128Prop(const std::string &Name, uint128_t Value) {
    //     return Property(Name, "u128", std::make_shared<U128>(Value));
    // }

    Property Property::StringProp(const std::string &Name, const std::string &Value)
    {
        return Property(Name, "0x1::string::String", std::make_shared<BString>(Value));
    }

    Property Property::BytesProp(const std::string &Name, const std::vector<uint8_t> &Value)
    {
        return Property(Name, "vector<u8>", std::make_shared<Bytes>(Value));
    }

    PropertyMap::PropertyMap(const std::vector<Property> &Properties)
        : Properties(Properties) {}

    std::string PropertyMap::ToString() const
    {
        std::string response = "PropertyMap[";
        for (const Property &prop : Properties)
            response += prop.ToString();

        if (!Properties.empty())
            response = response.substr(0, response.length() - 2);
        response += "]";
        return response;
    }

    std::tuple<std::vector<BString>, std::vector<BString>, std::vector<std::vector<uint8_t>>> PropertyMap::ToTuple() const
    {
        std::vector<BString> names;
        std::vector<BString> types;
        std::vector<std::vector<uint8_t>> values;

        for (const Property &prop : Properties)
        {
            names.push_back(prop.getName());
            types.push_back(prop.getPropertyType());
            values.push_back(prop.SerializeValue());
        }

        return std::make_tuple(names, types, values);
    }

    std::shared_ptr<IResource> PropertyMap::Parse(std::shared_ptr<AptosRESTModel::ResourceDataBase> resource)
    {
        using namespace AptosRESTModel;
        std::shared_ptr<PropertyMapResourceData> propMapResData =
            std::dynamic_pointer_cast<PropertyMapResourceData>(resource);
        std::vector<Property> properties;
        std::vector<PropertyResource> props = propMapResData->getInnerData().getData();

        for (const PropertyResource &prop : props)
        {
            properties.push_back(Property::Parse(
                prop.getKey(),
                std::stoi(prop.getValue().getType()),
                Bytes(Aptos::Utils::ByteArrayFromHexString(prop.getValue().getValue()))));
        }

        return std::make_shared<PropertyMap>(properties);
    }

    std::string PropertyMap::GetStructTag() const
    {
        return StructTag;
    }

    ReadObject::ReadObject(const std::unordered_map<std::string, std::shared_ptr<IResource>> &R1) : Resources(R1) {}

    std::string ReadObject::ToString() const
    {
        std::string response = "ReadObject";
        for (const auto &resourceObjValue : Resources)
        {
            response += "\n\t" + resourceObjValue.first + ": " + resourceObjValue.second->ToString();
        }
        return response;
    }

    AptosTokenClient::AptosTokenClient(RestClient &client) : m_restClient{client}
    {
    }

    void AptosTokenClient::ReadObject(std::function<void(Aptos::Rest::ReadObject, AptosRESTModel::ResponseInfo)> callback, const AccountAddress &address)
    {
        using namespace AptosRESTModel;
        bool success = false;
        long responseCode = 0;
        std::string resourcesResp = "";

        m_restClient.GetAccountResources([&success, &responseCode, &resourcesResp](bool _success, long _responseCode, std::string const &_resourcesResp)
                                         {
                                         success = _success;
                                         responseCode = _responseCode;
                                         resourcesResp = _resourcesResp; },
                                         address);

        AptosRESTModel::ResponseInfo responseInfo;
        if (!success && responseCode == 404)
        {
            responseInfo.status = AptosRESTModel::ResponseInfo::Status::Failed;
            responseInfo.message = resourcesResp;
            callback(Aptos::Rest::ReadObject{{}}, responseInfo);
            return;
        }
        std::unordered_map<std::string, std::shared_ptr<IResource>> resources;
        std::vector<std::shared_ptr<IResourceBase>> readResources = ResourceBaseListConverter::ReadJson(nlohmann::json::parse(resourcesResp));
        for (const auto &resource : readResources)
        {
            std::string type = resource->getType();

            if (Rest::ReadObject::ResourceMap.find(type) != Rest::ReadObject::ResourceMap.end())
            {
                std::string resourceObj = type;

                if (resourceObj == Collection::StructTag)
                {
                    auto collectionRes = std::dynamic_pointer_cast<CollectionResource>(resource);
                    CollectionResourceData data = collectionRes->getData();
                    std::shared_ptr<IResource> collection = Collection::Parse(std::make_shared<CollectionResourceData>(data));
                    resources[resourceObj] = collection;
                }
                else if (resourceObj == Object::StructTag)
                {
                    auto objectRes = std::dynamic_pointer_cast<ObjectResource>(resource);
                    ObjectResourceData data = objectRes->getData();
                    std::shared_ptr<IResource> obj = Object::Parse(std::make_shared<ObjectResourceData>(data));
                    resources[resourceObj] = obj;
                }
                else if (resourceObj == PropertyMap::StructTag)
                {
                    auto propMapRes = std::dynamic_pointer_cast<PropertyMapResource>(resource);
                    PropertyMapResourceData data = propMapRes->getData();
                    std::shared_ptr<IResource> propMap = PropertyMap::Parse(std::make_shared<PropertyMapResourceData>(data));
                    resources[resourceObj] = propMap;
                }
                else if (resourceObj == Royalty::StructTag)
                {
                    auto royaltyRes = std::dynamic_pointer_cast<RoyaltyResource>(resource);
                    RoyaltyResourceData data = royaltyRes->getData();
                    std::shared_ptr<IResource> royalty = Royalty::Parse(std::make_shared<RoyaltyResourceData>(data));
                    resources[resourceObj] = royalty;
                }
                else
                { // Token
                    auto tokenRes = std::dynamic_pointer_cast<TokenResource>(resource);
                    TokenResourceData data = tokenRes->getData();
                    std::shared_ptr<IResource> token = Token::Parse(std::make_shared<TokenResourceData>(data));
                    resources[resourceObj] = token;
                }
            }
        }
        responseInfo.status = ResponseInfo::Status::Success;
        responseInfo.message = resourcesResp;
        callback(Aptos::Rest::ReadObject(resources), responseInfo);
    }

    void AptosTokenClient::CreateCollection(std::function<void(std::string, AptosRESTModel::ResponseInfo)> callback, Account Creator, std::string Description, int MaxSupply, std::string Name, std::string Uri, bool MutableDescription, bool MutableRoyalty, bool MutableUri, bool MutableTokenDescription, bool MutableTokenName, bool MutableTokenProperties, bool MutableTokenUri, bool TokensBurnableByCreator, bool TokensFreezableByCreator, int RoyaltyNumerator, int RoyaltyDenominator)
    {

        std::vector<std::shared_ptr<ISerializable>> transactionArguments = {
            std::make_shared<BString>(Description),
            std::make_shared<U64>(static_cast<uint64_t>(MaxSupply)),
            std::make_shared<BString>(Name),
            std::make_shared<BString>(Uri),
            std::make_shared<Bool>(MutableDescription),
            std::make_shared<Bool>(MutableRoyalty),
            std::make_shared<Bool>(MutableUri),
            std::make_shared<Bool>(MutableTokenDescription),
            std::make_shared<Bool>(MutableTokenName),
            std::make_shared<Bool>(MutableTokenProperties),
            std::make_shared<Bool>(MutableTokenUri),
            std::make_shared<Bool>(TokensBurnableByCreator),
            std::make_shared<Bool>(TokensFreezableByCreator),
            std::make_shared<U64>(static_cast<uint64_t>(RoyaltyNumerator)),
            std::make_shared<U64>(static_cast<uint64_t>(RoyaltyDenominator))};

        EntryFunction payload = EntryFunction::Natural(ModuleId(AccountAddress::FromHex("0x4"), "aptos_token"),
                                                       "create_collection",
                                                       TagSequence{std::vector<std::shared_ptr<ISerializableTag>>{}},
                                                       Sequence{transactionArguments});

        std::shared_ptr<SignedTransaction> signedTransaction = nullptr;
        m_restClient.CreateBCSSignedTransaction(
            [&](std::shared_ptr<SignedTransaction> _signedTransaction)
            {
                signedTransaction = _signedTransaction;
            },
            Creator,
            BCS::TransactionPayload(std::make_shared<EntryFunction>(payload)));

        std::string submitBcsTxnJsonResponse = "";
        AptosRESTModel::ResponseInfo responseInfo;

        m_restClient.SubmitBCSTransaction(
            [&](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
            {
                submitBcsTxnJsonResponse = _responseJson;
                responseInfo = _responseInfo;
            },
            *signedTransaction);

        callback(submitBcsTxnJsonResponse, responseInfo);
    }

    void AptosTokenClient::MintToken(std::function<void(std::string, AptosRESTModel::ResponseInfo)> Callback, Account Creator, std::string Collection, std::string Description, std::string Name, std::string Uri, PropertyMap Properties)
    {
        auto propertiesTuple = Properties.ToTuple();

        std::vector<std::shared_ptr<ISerializable>> sq1, sq2;
        for (const auto b1 : std::get<0>(propertiesTuple))
        {
            sq1.push_back(std::make_shared<BString>(b1));
        }
        for (const auto b2 : std::get<1>(propertiesTuple))
        {
            sq2.push_back(std::make_shared<BString>(b2));
        }

        std::vector<std::shared_ptr<ISerializable>> transactionArguments = {
            std::make_shared<BString>(Collection),
            std::make_shared<BString>(Description),
            std::make_shared<BString>(Name),
            std::make_shared<BString>(Uri),
            std::make_shared<Sequence>(sq1),
            std::make_shared<Sequence>(sq2),
            std::make_shared<BytesSequence>(std::get<2>(propertiesTuple))};

        EntryFunction payload = EntryFunction::Natural(ModuleId(AccountAddress::FromHex("0x4"), "aptos_token"),
                                                       "mint",
                                                       TagSequence{std::vector<std::shared_ptr<ISerializableTag>>{}},
                                                       Sequence{transactionArguments});

        std::shared_ptr<SignedTransaction> signedTransaction = nullptr;
        m_restClient.CreateBCSSignedTransaction(
            [&](std::shared_ptr<SignedTransaction> _signedTransaction)
            {
                signedTransaction = _signedTransaction;
            },
            Creator,
            BCS::TransactionPayload(std::make_shared<EntryFunction>(payload)));

        std::string submitBcsTxnJsonResponse = "";
        AptosRESTModel::ResponseInfo responseInfo;

        m_restClient.SubmitBCSTransaction(
            [&](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
            {
                submitBcsTxnJsonResponse = _responseJson;
                responseInfo = _responseInfo;
            },
            *signedTransaction);

        Callback(submitBcsTxnJsonResponse, responseInfo);
    }

    void AptosTokenClient::IMintSoulBoundToken(std::function<void(std::string, AptosRESTModel::ResponseInfo)> Callback, Account Creator, std::string Collection, std::string Description, std::string Name, std::string Uri, PropertyMap Properties, AccountAddress SoulBoundTo)
    {
        auto propertiesTuple = Properties.ToTuple();

        std::vector<std::shared_ptr<ISerializable>> sq1, sq2;
        for (const auto b1 : std::get<0>(propertiesTuple))
        {
            sq1.push_back(std::make_shared<BString>(b1));
        }
        for (const auto b2 : std::get<1>(propertiesTuple))
        {
            sq2.push_back(std::make_shared<BString>(b2));
        }

        std::vector<std::shared_ptr<ISerializable>> transactionArguments = {
            std::make_shared<BString>(Collection),
            std::make_shared<BString>(Description),
            std::make_shared<BString>(Name),
            std::make_shared<BString>(Uri),
            std::make_shared<Sequence>(sq1),
            std::make_shared<Sequence>(sq2),
            std::make_shared<BytesSequence>(std::get<2>(propertiesTuple)),
            std::make_shared<AccountAddress>(SoulBoundTo)};

        EntryFunction payload = EntryFunction::Natural(ModuleId(AccountAddress::FromHex("0x4"), "aptos_token"),
                                                       "mint_soul_bound",
                                                       TagSequence{std::vector<std::shared_ptr<ISerializableTag>>{}},
                                                       Sequence{transactionArguments});

        std::shared_ptr<SignedTransaction> signedTransaction = nullptr;
        m_restClient.CreateBCSSignedTransaction(
            [&](std::shared_ptr<SignedTransaction> _signedTransaction)
            {
                signedTransaction = _signedTransaction;
            },
            Creator,
            BCS::TransactionPayload(std::make_shared<EntryFunction>(payload)));

        std::string submitBcsTxnJsonResponse = "";
        AptosRESTModel::ResponseInfo responseInfo;

        m_restClient.SubmitBCSTransaction(
            [&](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
            {
                submitBcsTxnJsonResponse = _responseJson;
                responseInfo = _responseInfo;
            },
            *signedTransaction);

        Callback(submitBcsTxnJsonResponse, responseInfo);
    }

    void AptosTokenClient::TransferToken(std::function<void(std::string, AptosRESTModel::ResponseInfo)> Callback, Account Owner, AccountAddress Token, AccountAddress To)
    {
        return m_restClient.TransferObject(Callback, Owner, Token, To);
    }

    void AptosTokenClient::BurnToken(std::function<void(std::string, AptosRESTModel::ResponseInfo)> Callback, Account Creator, AccountAddress Token)
    {
        StructTag tokenStructTag(
            AccountAddress::FromHex("0x4"),
            "token",
            "Token",
            {});
        std::vector<std::shared_ptr<ISerializable>> transactionArguments = {
            std::make_shared<AccountAddress>(Token),
        };
        EntryFunction payload = EntryFunction::Natural(
            ModuleId(AccountAddress::FromHex("0x4"), "aptos_token"),
            "burn",
            TagSequence{{std::make_shared<StructTag>(tokenStructTag)}},
            Sequence{transactionArguments});

        std::shared_ptr<SignedTransaction> signedTransaction = nullptr;
        m_restClient.CreateBCSSignedTransaction(
            [&](std::shared_ptr<SignedTransaction> _signedTransaction)
            {
                signedTransaction = _signedTransaction;
            },
            Creator,
            BCS::TransactionPayload(std::make_shared<EntryFunction>(payload)));

        std::string submitBcsTxnJsonResponse = "";
        AptosRESTModel::ResponseInfo responseInfo;
        m_restClient.SubmitBCSTransaction(
            [&](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
            {
                submitBcsTxnJsonResponse = _responseJson;
                responseInfo = _responseInfo;
            },
            *signedTransaction);

        Callback(submitBcsTxnJsonResponse, responseInfo);
    }

    void AptosTokenClient::FreezeToken(std::function<void(std::string, AptosRESTModel::ResponseInfo)> Callback, Account Creator, AccountAddress Token)
    {
        StructTag tokenStructTag(
            AccountAddress::FromHex("0x4"),
            "token",
            "Token",
            {});

        EntryFunction payload = EntryFunction::Natural(
            ModuleId(AccountAddress::FromHex("0x4"), "aptos_token"),
            "freeze_transfer",
            TagSequence{{std::make_shared<StructTag>(tokenStructTag)}},
            Sequence{{std::make_shared<AccountAddress>(Token)}});

        std::shared_ptr<SignedTransaction> signedTransaction = nullptr;
        m_restClient.CreateBCSSignedTransaction(
            [&](std::shared_ptr<SignedTransaction> _signedTransaction)
            {
                signedTransaction = _signedTransaction;
            },
            Creator,
            BCS::TransactionPayload(std::make_shared<EntryFunction>(payload)));

        std::string submitBcsTxnJsonResponse = "";
        AptosRESTModel::ResponseInfo responseInfo;
        m_restClient.SubmitBCSTransaction(
            [&](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
            {
                submitBcsTxnJsonResponse = _responseJson;
                responseInfo = _responseInfo;
            },
            *signedTransaction);

        Callback(submitBcsTxnJsonResponse, responseInfo);
    }

    void AptosTokenClient::UnfreezeToken(std::function<void(std::string, AptosRESTModel::ResponseInfo)> Callback, Account Creator, AccountAddress Token)
    {
        StructTag tokenStructTag = StructTag(
            AccountAddress::FromHex("0x4"),
            "token",
            "Token",
            {});

        EntryFunction payload = EntryFunction::Natural(
            ModuleId(AccountAddress::FromHex("0x4"), "aptos_token"),
            "unfreeze_transfer",
            TagSequence{{std::make_shared<StructTag>(tokenStructTag)}},
            Sequence{{std::make_shared<AccountAddress>(Token)}});

        std::shared_ptr<SignedTransaction> signedTransaction = nullptr;
        m_restClient.CreateBCSSignedTransaction(
            [&](std::shared_ptr<SignedTransaction> _signedTransaction)
            {
                signedTransaction = _signedTransaction;
            },
            Creator,
            BCS::TransactionPayload(std::make_shared<EntryFunction>(payload)));

        std::string submitBcsTxnJsonResponse = "";
        AptosRESTModel::ResponseInfo responseInfo;
        m_restClient.SubmitBCSTransaction(
            [&](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
            {
                submitBcsTxnJsonResponse = _responseJson;
                responseInfo = _responseInfo;
            },
            *signedTransaction);

        Callback(submitBcsTxnJsonResponse, responseInfo);
    }

    void AptosTokenClient::AddTokenProperty(std::function<void(std::string, AptosRESTModel::ResponseInfo)> Callback, Account Creator, AccountAddress Token, Property Prop)
    {
        auto txnArgumentsList = Prop.ToTransactionArguments();
        txnArgumentsList.insert(txnArgumentsList.begin(), std::make_shared<AccountAddress>(Token));
        std::vector<std::shared_ptr<ISerializable>> transactionArguments(txnArgumentsList.begin(), txnArgumentsList.end());

        StructTag tokenStructTag = StructTag(
            AccountAddress::FromHex("0x4"),
            "token",
            "Token",
            {});

        EntryFunction payload = EntryFunction::Natural(
            ModuleId(AccountAddress::FromHex("0x4"), "aptos_token"),
            "add_property",
            TagSequence{{std::make_shared<StructTag>(tokenStructTag)}},
            Sequence(transactionArguments));

        std::shared_ptr<SignedTransaction> signedTransaction = nullptr;
        m_restClient.CreateBCSSignedTransaction(
            [&](std::shared_ptr<SignedTransaction> _signedTransaction)
            {
                signedTransaction = _signedTransaction;
            },
            Creator,
            BCS::TransactionPayload(std::make_shared<EntryFunction>(payload)));

        std::string submitBcsTxnJsonResponse = "";
        AptosRESTModel::ResponseInfo responseInfo;
        m_restClient.SubmitBCSTransaction(
            [&](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
            {
                submitBcsTxnJsonResponse = _responseJson;
                responseInfo = _responseInfo;
            },
            *signedTransaction);

        Callback(submitBcsTxnJsonResponse, responseInfo);
    }

    void AptosTokenClient::RemoveTokenProperty(std::function<void(std::string, AptosRESTModel::ResponseInfo)> Callback, Account Creator, AccountAddress Token, std::string Name)
    {
        std::vector<std::shared_ptr<ISerializable>> transactionArguments = {
            std::make_shared<AccountAddress>(Token),
            std::make_shared<BString>(Name)};

        StructTag tokenStructTag = StructTag(
            AccountAddress::FromHex("0x4"),
            "token",
            "Token",
            {});

        EntryFunction payload = EntryFunction::Natural(
            ModuleId(AccountAddress::FromHex("0x4"), "aptos_token"),
            "remove_property",
            TagSequence{{std::make_shared<StructTag>(tokenStructTag)}},
            Sequence(transactionArguments));

        std::shared_ptr<SignedTransaction> signedTransaction = nullptr;
        m_restClient.CreateBCSSignedTransaction(
            [&](std::shared_ptr<SignedTransaction> _signedTransaction)
            {
                signedTransaction = _signedTransaction;
            },
            Creator,
            BCS::TransactionPayload(std::make_shared<EntryFunction>(payload)));

        std::string submitBcsTxnJsonResponse = "";
        AptosRESTModel::ResponseInfo responseInfo;
        m_restClient.SubmitBCSTransaction(
            [&](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
            {
                submitBcsTxnJsonResponse = _responseJson;
                responseInfo = _responseInfo;
            },
            *signedTransaction);

        Callback(submitBcsTxnJsonResponse, responseInfo);
    }

    void AptosTokenClient::UpdateTokenProperty(std::function<void(std::string, AptosRESTModel::ResponseInfo)> Callback, Account Creator, AccountAddress Token, Property Prop)
    {
        auto txnArgumentsList = Prop.ToTransactionArguments();
        txnArgumentsList.insert(txnArgumentsList.begin(), std::make_shared<AccountAddress>(Token));
        std::vector<std::shared_ptr<ISerializable>> transactionArguments(txnArgumentsList.begin(), txnArgumentsList.end());

        StructTag tokenStructTag = StructTag(
            AccountAddress::FromHex("0x4"),
            "token",
            "Token",
            {});

        EntryFunction payload = EntryFunction::Natural(
            ModuleId(AccountAddress::FromHex("0x4"), "aptos_token"),
            "update_property",
            TagSequence{{std::make_shared<StructTag>(tokenStructTag)}},
            Sequence(transactionArguments));

        std::shared_ptr<SignedTransaction> signedTransaction = nullptr;
        m_restClient.CreateBCSSignedTransaction(
            [&](std::shared_ptr<SignedTransaction> _signedTransaction)
            {
                signedTransaction = _signedTransaction;
            },
            Creator,
            BCS::TransactionPayload(std::make_shared<EntryFunction>(payload)));

        std::string submitBcsTxnJsonResponse = "";
        AptosRESTModel::ResponseInfo responseInfo;
        m_restClient.SubmitBCSTransaction(
            [&](std::string _responseJson, AptosRESTModel::ResponseInfo _responseInfo)
            {
                submitBcsTxnJsonResponse = _responseJson;
                responseInfo = _responseInfo;
            },
            *signedTransaction);

        Callback(submitBcsTxnJsonResponse, responseInfo);
    }

    void AptosTokenClient::TokensMintedFromTransaction(std::function<void(std::vector<AccountAddress>, AptosRESTModel::ResponseInfo)> Callback, std::string TransactionHash)
    {
        AptosRESTModel::ResponseInfo responseInfo;
        AptosRESTModel::Transaction responseTx;

        m_restClient.TransactionByHash(
            [&](AptosRESTModel::Transaction _responseTx, AptosRESTModel::ResponseInfo _responseInfo)
            {
                responseTx = _responseTx;
                responseInfo = _responseInfo;
            },
            TransactionHash);

        std::vector<AccountAddress> mints;

        for (const AptosRESTModel::TransactionEvent &txEvent : responseTx.getEvents())
        {
            if (txEvent.getType() == Constants::APTOS_MINT_EVENT)
            {
                mints.push_back(AccountAddress::FromHex(txEvent.getData().getToken()));
            }
        }

        Callback(mints, responseInfo);
    }

}
