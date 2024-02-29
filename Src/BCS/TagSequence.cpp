#include "TagSequence.h"
#include <vector>
#include "Serialization.h"
#include "Deserialization.h"
#include <sstream>

namespace Aptos::BCS
{
    TagSequence::TagSequence(const std::vector<std::shared_ptr<ISerializableTag>> &serializableTags)
    : serializableTags(serializableTags)
    {
    }
    
    void TagSequence::Serialize(Serialization &serializer) const
    {
        serializer.SerializeU32AsUleb128(this->serializableTags.size());
        for (const auto &element : this->serializableTags)
        {
            element->Serialize(serializer);
        }
    }

    std::shared_ptr<TagSequence> TagSequence::Deserialize(Deserialization &deserializer)
    {
        int length = deserializer.DeserializeUleb128();

        std::vector<std::shared_ptr<ISerializableTag>> values;

        while (values.size() < length)
        {
            auto tag = ISerializableTag::DeserializeTag(deserializer);
            values.push_back(std::dynamic_pointer_cast<ISerializableTag>(tag));
        }

        return std::make_shared<TagSequence>(values);
    }

    std::vector<std::shared_ptr<ISerializableTag>> TagSequence::GetValue() const
    {
        return serializableTags;
    }

    bool TagSequence::Equals(const TagSequence &other) const
    {
        Serialization s1;
        this->Serialize(s1);
        Serialization s2;
        other.Serialize(s2);
        return s1.GetBytes() == s2.GetBytes();
    }

    std::string TagSequence::ToString() const
    {
        std::ostringstream oss;
        for (const auto &tag : this->serializableTags)
        {
            oss << tag->ToString();
        }
        return oss.str();
    }

    size_t TagSequence::GetHashCode() const
    {
        return 0;
    }

    bool operator==(const TagSequence &lhs, const TagSequence &rhs)
    {
        return lhs.Equals(rhs);
    }
}
