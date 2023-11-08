//
// Created by Anh NPH on 22/09/2023.
//

#ifndef APTOS_BSTRING_H
#define APTOS_BSTRING_H
#include "BCSTypes.h"

class BString : public ISerializable {
private:
    std::string value;

public:
    BString();
    BString(const std::string& value);

    void Serialize(Serialization& serializer) const override;

    static std::string Deserialize(const std::vector<uint8_t>& data);

    static std::shared_ptr<ISerializable> Deserialize(Deserialization& deserializer);

    bool Equals(const BString& other) const;

    std::string ToString() const override;

    size_t GetHashCode() const;

    std::string GetValue() const;

    std::vector<uint8_t> RemoveBOM(const std::vector<uint8_t>& data);

    bool operator<(const BString& other) const;

    bool operator==(const BString& other) const;
};



#endif //APTOS_BSTRING_H
