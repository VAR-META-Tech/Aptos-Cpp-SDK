#ifndef ENTRYFUNCTION_H
#define ENTRYFUNCTION_H

#include <string>
#include <list>
#include "TagSequence.h"
#include "Sequence.h"
#include "ModuleId.h"

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of an entry function.
    /// </summary>
    class EntryFunction : public ISerializable
    {
    public:
        explicit EntryFunction(const ModuleId &moduleId, const std::string &function,
                               const TagSequence &typeArgs, const Sequence &args);
        /// <summary>
        /// Converts a Sequence of ISerializable objects and converts it to a Sequence of Bytes objects
        /// </summary>
        /// <param name="module"></param>
        /// <param name="function"></param>
        /// <param name="typeArgs"></param>
        /// <param name="args"></param>
        /// <returns></returns>
        static EntryFunction Natural(const ModuleId &moduleId, const std::string &function,
                                     const TagSequence &typeArgs, const Sequence &args);
        void Serialize(Serialization &serializer) const override;
        static std::shared_ptr<ISerializable> Deserialize(Deserialization &deserializer);
        bool Equals(const EntryFunction &other) const;
        std::string ToString() const override;
        size_t GetHashCode() const override;

    private:
        ModuleId moduleId;
        std::string function;
        TagSequence typeArgs;
        Sequence args;
    };
}
#endif // ENTRYFUNCTION_H
