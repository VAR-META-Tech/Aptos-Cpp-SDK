#ifndef MULTIPUBLICKEY_H
#define MULTIPUBLICKEY_H


#include <vector>
#include <stdexcept>
#include "PublicKey.h"
#include "../BCS/BCSTypes.h"

class MultiPublicKey : public ISerializable {
public:
    static const int MIN_KEYS = 2;
    static const int MAX_KEYS = 32;
    static const int MIN_THRESHOLD = 1;

    MultiPublicKey(const std::vector<PublicKey>& keys, CryptoPP::byte threshold, bool checked = true);

    std::vector<uint8_t> ToBytes() const;
    static MultiPublicKey FromBytes(const std::vector<uint8_t>& keyBytes);
    void Serialize(Serialization &serializer) const;
    std::string ToString() const;

private:
    std::vector<PublicKey> Keys;
    CryptoPP::byte Threshold;

};

#endif // MULTIPUBLICKEY_H
