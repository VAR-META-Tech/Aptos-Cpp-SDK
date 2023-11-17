#include "MultiAgentRawTransaction.h"

#include <stdexcept>
#include "../HDWallet/Utils/Utils.h"
#include <cryptopp/sha3.h>

MultiAgentRawTransaction::MultiAgentRawTransaction(const RawTransaction &rawTransaction, const Sequence &secondarySigners)
    : rawTransaction(rawTransaction), secondarySigners(secondarySigners) {
}

RawTransaction MultiAgentRawTransaction::Inner() {
    return this->rawTransaction;
}

std::vector<uint8_t> MultiAgentRawTransaction::Prehash() {
    CryptoPP::SHA3_256 sha3;
    std::string input = "APTOS::RawTransactionWithData";
    auto inputS = Utils::StringToSecByteBlock(input.data());
    sha3.Update(inputS, inputS.size());
    CryptoPP::SecByteBlock result(Utils::Ed25519PublicKeySizeInBytes); // 256 bits = 32 bytes
    sha3.Final(result);

    return Utils::SecBlockToByteVector(result);
}

std::vector<uint8_t> MultiAgentRawTransaction::Keyed() {
    Serialization ser;

    // This is a type indicator for an enum
    ser.SerializeU8(0);
    this->rawTransaction.Serialize(ser);
    this->secondarySigners.Serialize(ser);  // Assuming Sequence is serializable

    std::vector<uint8_t> prehash = this->Prehash();
    std::vector<uint8_t> outputBytes = ser.GetBytes();
    std::vector<uint8_t> res(prehash.size() + outputBytes.size());

    std::copy(prehash.begin(), prehash.end(), res.begin());
    std::copy(outputBytes.begin(), outputBytes.end(), res.begin() + prehash.size());

    return res;
}

Signature MultiAgentRawTransaction::Sign(PrivateKey key) {
    return key.Sign(Utils::ByteVectorToSecBlock(this->Keyed()));
}

bool MultiAgentRawTransaction::Verify(const PublicKey &key, const Signature &signature) {
    return key.Verify(Utils::ByteVectorToSecBlock(this->Keyed()), signature);
}