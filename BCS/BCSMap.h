//
// Created by Anh NPH on 22/09/2023.
//

#ifndef APTOS_BCSMAP_H
#define APTOS_BCSMAP_H
#include "BCSTypes.h"
#include "BString.h"
#include <map>

class BCSMap : public ISerializable {
private:
    std::map<BString, ISerializable*> values;

public:
    BCSMap(const std::map<BString, ISerializable*>& values);

    void Serialize(Serialization& serializer) override;

    void* GetValue();

    std::string ToString() const;
};

#endif //APTOS_BCSMAP_H
