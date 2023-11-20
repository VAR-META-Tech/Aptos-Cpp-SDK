#include "EntryFunction.h"
#include <stdexcept>
#include "Bytes.h"

namespace Aptos::BCS
{
    EntryFunction::EntryFunction(const ModuleId &module, const std::string &function, const TagSequence &typeArgs, const Sequence &args)
        : module(module), function(function), typeArgs(typeArgs), args(args) {}

    EntryFunction EntryFunction::Natural(const ModuleId &module, const std::string &function, const TagSequence &typeArgs, const Sequence &args)
    {
        auto value = args.GetValue();

        std::vector<std::shared_ptr<ISerializable>> valuesAsBytes;

        for (const auto &element : value)
        {
            if (std::dynamic_pointer_cast<Sequence>(element))
            {
                Serialization seqSerializer;
                auto seq = std::dynamic_pointer_cast<Sequence>(element);
                seqSerializer.Serialize(*seq);

                auto bytes = seqSerializer.GetBytes();
                valuesAsBytes.push_back(std::make_shared<Bytes>(bytes));
            }
            else
            {
                Serialization s;
                element->Serialize(s);
                auto bytes = s.GetBytes();
                valuesAsBytes.push_back(std::make_shared<Bytes>(bytes));
            }
        }

        Sequence seqBytes(valuesAsBytes);
        return EntryFunction(module, function, typeArgs, seqBytes);
    }

    void EntryFunction::Serialize(Serialization &serializer) const
    {
        serializer.Serialize(module);
        serializer.SerializeString(function);
        serializer.Serialize(typeArgs);
        serializer.Serialize(args);
    }

    std::shared_ptr<ISerializable> EntryFunction::Deserialize(Deserialization &deserializer)
    {
        auto module = std::dynamic_pointer_cast<ModuleId>(ModuleId::Deserialize(deserializer));
        std::string function = deserializer.DeserializeString();
        auto typeArgs = std::dynamic_pointer_cast<TagSequence>(deserializer.DeserializeTagSequence());
        auto args = std::dynamic_pointer_cast<Sequence>(Sequence::Deserialize(deserializer));
        return std::make_shared<EntryFunction>(*module, function, *typeArgs, *args);
    }

    bool EntryFunction::Equals(const EntryFunction &other) const
    {
        bool moduleComparison = module.Equals(other.module);
        bool entryFuncComparison = function == other.function;
        bool typeArgsComparison = typeArgs.Equals(other.typeArgs);
        bool argsComparison = args.Equals(other.args);
        return moduleComparison && entryFuncComparison && typeArgsComparison && argsComparison;
    }

    std::string EntryFunction::ToString() const
    {
        return module.ToString() + "::" + function + "<" + typeArgs.ToString() + ">(" + args.ToString() + ")";
    }

    size_t EntryFunction::GetHashCode() const
    {
        size_t hash = 17;
        hash = hash * 23 + module.GetHashCode();
        hash = hash * 23 + std::hash<std::string>{}(function);
        hash = hash * 23 + typeArgs.GetHashCode();
        hash = hash * 23 + args.GetHashCode();
        return hash;
    }
}