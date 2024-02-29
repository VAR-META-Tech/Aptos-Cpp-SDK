//
// Created by Anh NPH on 21/09/2023.
//

#include "Sequence.h"
#include <sstream>
#include <functional>
#include "Bytes.h"
#include "Serialization.h"
#include "Deserialization.h"

namespace Aptos::BCS
{
    Sequence::Sequence(const std::vector<std::shared_ptr<ISerializable>> &values): values(values)
    {
    }

    int Sequence::Length() const
    {
        return this->values.size();
    }

    std::vector<std::shared_ptr<ISerializable>> Sequence::GetValue() const
    {
        return values;
    }

    void Sequence::Serialize(Serialization &serializer) const
    {
        serializer.SerializeU32AsUleb128(this->values.size());
        for (const auto &element : this->values)
        {
            if (std::dynamic_pointer_cast<Sequence>(element))
            {
                Serialization seqSerializer;
                auto seq = std::dynamic_pointer_cast<Sequence>(element);
                seqSerializer.Serialize(*seq);

                std::vector<uint8_t> elementsBytes = seqSerializer.GetBytes();
                int sequenceLen = elementsBytes.size();
                serializer.SerializeU32AsUleb128(sequenceLen);
                serializer.SerializeFixedBytes(elementsBytes);
            }
            else
            {
                Serialization s;
                element->Serialize(s);
                std::vector<uint8_t> b = s.GetBytes();
                serializer.SerializeBytes(b);
            }
        }
    }

    std::shared_ptr<Sequence> Sequence::Deserialize(Deserialization &deserializer)
    {
        int length = deserializer.DeserializeUleb128();
        std::vector<std::shared_ptr<ISerializable>> values;

        while (values.size() < length)
        {
            values.push_back(std::make_shared<Bytes>(deserializer.ToBytes()));
        }

        return std::make_shared<Sequence>(values);
    }

    bool Sequence::Equals(const Sequence &other) const
    {
        Serialization s1;
        this->Serialize(s1);
        Serialization s2;
        other.Serialize(s2);
        return s1.GetBytes() == s2.GetBytes();
    }

    std::string Sequence::ToString() const
    {
        std::ostringstream oss;
        for (const auto &value : this->values)
        {
            oss << value->ToString();
        }
        return oss.str();
    }

    size_t Sequence::GetHashCode() const
    {
        return 0;
    }

    const std::vector<std::shared_ptr<ISerializable>> &Sequence::getValues() const
    {
        return values;
    }
}
