//
// Created by Anh NPH on 22/09/2023.
//

#ifndef APTOS_BCSMAP_H
#define APTOS_BCSMAP_H
#include "BCSTypes.h"
#include "BString.h"
#include <map>

namespace Aptos::BCS
{
    /// <summary>
    /// Representation of a map in BCS.
    /// </summary>
    class APTOS_API BCSMap : public ISerializable
    {
    private:
        /// <summary>
        /// A dictionary mapping to values that are serializable.
        /// </summary>
        std::map<BString, std::shared_ptr<ISerializable>> values;

    public:
        /// <summary>
        /// Creates a BCSMap from a Dictionary.
        /// </summary>
        /// <param name="values">A dictionary mapping to values that are
        /// serializable.</param>
        explicit BCSMap(const std::map<BString, std::shared_ptr<ISerializable>> &values);

        /// <summary>
        /// Maps (Key / Value Stores)
        /// Maps are represented as a variable-length, sorted sequence of(Key, Value) tuples.
        /// Keys must be unique and the tuples sorted by increasing lexicographical order on
        /// the BCS bytes of each key.
        /// The representation is otherwise similar to that of a variable-length sequence.
        /// In particular, it is preceded by the number of tuples, encoded in ULEB128.
        /// </summary>
        /// <param name="serializer"></param>
        void Serialize(Serialization &serializer) const override;

        /// <inheritdoc/>
        std::string ToString() const override;

        /// <summary>
        /// Gets the internal dictionary of a BCSMap.
        /// </summary>
        /// <returns></returns>
        std::map<BString, std::shared_ptr<ISerializable>> getValues() const;
    };
}
#endif // APTOS_BCSMAP_H
