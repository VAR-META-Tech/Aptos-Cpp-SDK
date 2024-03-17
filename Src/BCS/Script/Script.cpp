#include "Script.h"
#include <stdexcept>
#include "../Serialization.h"
#include "../Deserialization.h"
#include <sstream>

namespace Aptos::BCS
{
    Script::Script(const std::vector<uint8_t> &code, const TagSequence &typeArgs, const Sequence &scriptArgs)
        : code(code), typeArgs(typeArgs), scriptArgs(scriptArgs)
    {
    }

    void Script::Serialize(Serialization &serializer) const
    {
        serializer.SerializeBytes(this->code);
        serializer.Serialize(this->typeArgs);
        serializer.Serialize(this->scriptArgs);
    }

    std::shared_ptr<Script> Script::Deserialize(Deserialization &deserializer)
    {
        std::vector<uint8_t> code = deserializer.ToBytes();
        auto typeArgs = deserializer.DeserializeTagSequence();
        auto scriptArgs = deserializer.DeserializeScriptArgSequence();
        return std::make_shared<Script>(code, *typeArgs, *scriptArgs);
    }

    bool Script::Equals(const Script &other) const
    {
        bool codeComparison = this->code == other.code;
        bool typeArgsComparison = this->typeArgs.Equals(other.typeArgs);
        bool scriptArgsComparison = this->scriptArgs.Equals(other.scriptArgs);
        return codeComparison && typeArgsComparison && scriptArgsComparison;
    }

    std::string Script::ToString()
    {
        return "<" + this->typeArgs.ToString() + ">(" + this->scriptArgs.ToString() + ")";
    }

    size_t Script::GetHashCode() const
    {
        size_t hash = 17;
        for (uint8_t byte : this->code)
        {
            hash = hash * 23 + static_cast<size_t>(byte);
        }
        hash = hash * 23 + this->typeArgs.GetHashCode();
        hash = hash * 23 + this->scriptArgs.GetHashCode();
        return hash;
    }

    std::string Script::ToString() const
    {
        std::stringstream ss;
        ss << "<" << this->typeArgs.ToString() << ">(" << this->scriptArgs.ToString() << ")";
        return ss.str();
    }

    bool operator==(const Script &lhs, const Script &rhs)
    {
        return lhs.Equals(rhs);
    }
}