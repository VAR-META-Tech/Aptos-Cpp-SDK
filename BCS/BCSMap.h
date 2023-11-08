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

    void Serialize(Serialization& serializer) const override;

    std::string ToString() const override;
};

#endif //APTOS_BCSMAP_H
