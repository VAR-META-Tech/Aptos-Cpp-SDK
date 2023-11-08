#ifndef SCRIPT_H
#define SCRIPT_H

#include <vector>
#include <string>
#include "Sequence.h"
#include "TagSequence.h"
#include <cstdint>

class Script : public ISerializable{
public:
    Script(const std::vector<uint8_t>& code, const TagSequence& typeArgs, const Sequence& scriptArgs);

    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    bool Equals(const Script& other) const;
    std::string ToString();
    size_t GetHashCode() override;
    std::string ToString() const override;
private:
    std::vector<uint8_t> code;
    TagSequence typeArgs;
    Sequence scriptArgs;
};

bool operator==(const Script &lhs, const Script &rhs);


#endif // SCRIPT_H
