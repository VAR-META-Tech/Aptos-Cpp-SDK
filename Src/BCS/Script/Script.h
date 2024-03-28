#ifndef SCRIPT_H
#define SCRIPT_H

#include <vector>
#include <string>
#include "../Sequence.h"
#include "../TagSequence.h"
#include <cstdint>

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a script passed as bytes.
    /// </summary>
    class APTOS_API Script : public ISerializable
    {
    public:
        explicit Script(const std::vector<uint8_t> &code, const TagSequence &typeArgs, const Sequence &scriptArgs);

        void Serialize(Serialization &serializer) const override;
        static std::shared_ptr<Script> Deserialize(Deserialization &deserializer);
        bool Equals(const Script &other) const;
        std::string ToString();
        size_t GetHashCode() const override;
        std::string ToString() const override;

    private:
        std::vector<uint8_t> code;
        TagSequence typeArgs;
        Sequence scriptArgs;
    };

    bool APTOS_API operator==(const Script &lhs, const Script &rhs);
}
#endif // SCRIPT_H
