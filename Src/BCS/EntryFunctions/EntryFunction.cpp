#include "EntryFunction.h"
#include <stdexcept>
#include "../Bytes.h"

namespace Aptos::BCS
{

    EntryFunction::EntryFunction(const ModuleId &moduleId, const std::string &function, const TagSequence &typeArgs, const Sequence &args)
        : moduleId(moduleId), function(function), typeArgs(typeArgs), args(args) {}

    EntryFunction EntryFunction::Natural(const ModuleId &moduleId, const std::string &function, const TagSequence &typeArgs, const Sequence &args)
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
        return EntryFunction(moduleId, function, typeArgs, seqBytes);
    }

    void EntryFunction::Serialize(Serialization &serializer) const
    {
        serializer.Serialize(moduleId);
        serializer.SerializeString(function);
        serializer.Serialize(typeArgs);
        serializer.Serialize(args);
    }

    std::shared_ptr<EntryFunction> EntryFunction::Deserialize(Deserialization &deserializer)
    {
        auto moduleId = std::dynamic_pointer_cast<ModuleId>(ModuleId::Deserialize(deserializer));
        std::string function = deserializer.DeserializeString();
        auto typeArgs = std::dynamic_pointer_cast<TagSequence>(deserializer.DeserializeTagSequence());
        auto args = std::dynamic_pointer_cast<Sequence>(Sequence::Deserialize(deserializer));
        return std::make_shared<EntryFunction>(*moduleId, function, *typeArgs, *args);
    }

    bool EntryFunction::Equals(const EntryFunction &other) const
    {
        bool moduleComparison = moduleId.Equals(other.moduleId);
        bool entryFuncComparison = function == other.function;
        bool typeArgsComparison = typeArgs.Equals(other.typeArgs);
        bool argsComparison = args.Equals(other.args);
        return moduleComparison && entryFuncComparison && typeArgsComparison && argsComparison;
    }

    std::string EntryFunction::ToString() const
    {
        return moduleId.ToString() + "::" + function + "<" + typeArgs.ToString() + ">(" + args.ToString() + ")";
    }

    size_t EntryFunction::GetHashCode() const
    {
        size_t hash = 17;
        hash = hash * 23 + moduleId.GetHashCode();
        hash = hash * 23 + std::hash<std::string>{}(function);
        hash = hash * 23 + typeArgs.GetHashCode();
        hash = hash * 23 + args.GetHashCode();
        return hash;
    }

    ModuleId EntryFunction::getModule() const
    {
        return moduleId;
    }

    void EntryFunction::setModule(const ModuleId &newModule)
    {
        moduleId = newModule;
    }

    std::string EntryFunction::getFunction() const
    {
        return function;
    }

    void EntryFunction::setFunction(const std::string &newFunction)
    {
        function = newFunction;
    }

    TagSequence EntryFunction::getTypeArgs() const
    {
        return typeArgs;
    }

    void EntryFunction::setTypeArgs(const TagSequence &newTypeArgs)
    {
        typeArgs = newTypeArgs;
    }

    Sequence EntryFunction::getArgs() const
    {
        return args;
    }

    void EntryFunction::setArgs(const Sequence &newArgs)
    {
        args = newArgs;
    }
}
