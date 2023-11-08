//
// Created by Anh NPH on 21/09/2023.
//

#ifndef APTOS_SEQUENCE_H
#define APTOS_SEQUENCE_H
#include "BCSTypes.h"
#include <memory>

class Sequence : public ISerializable {

public:
    Sequence(const std::vector<std::shared_ptr<ISerializable>>& values);
    int Length() const;
    std::vector<std::shared_ptr<ISerializable>> GetValue() const;
    void Serialize(Serialization& serializer) const override;
    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);
    bool Equals(const Sequence& other) const;
    std::string ToString() const override;
    size_t GetHashCode() const;    
private:
    std::vector<std::shared_ptr<ISerializable>> values;
};

#endif //APTOS_SEQUENCE_H
