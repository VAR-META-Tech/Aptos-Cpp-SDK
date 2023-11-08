#ifndef ENTRYFUNCTION_H
#define ENTRYFUNCTION_H


#include <string>
#include <list>
#include "TagSequence.h"
#include "Sequence.h"
#include "ModuleId.h"

class EntryFunction : public ISerializable {
public:
    EntryFunction(const ModuleId& module, const std::string& function,
                  const TagSequence& typeArgs, const Sequence& args);
    static EntryFunction Natural(const ModuleId& module, const std::string& function,
                                 const TagSequence& typeArgs, const Sequence& args);
    void Serialize(Serialization& serializer) const  override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    bool Equals(const EntryFunction& other) const;
    std::string ToString() const;
    size_t GetHashCode() const;
private:
    ModuleId module;
    std::string function;
    TagSequence typeArgs;
    Sequence args;
};


#endif // ENTRYFUNCTION_H
