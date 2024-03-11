#include <iostream>
#include "AptosToken/AptosToken.h"
#include "TransferCoin/TransferCoinExample.h"
#include "SimulateTransferCoin/SimulateTransferCoin.h"
#include "SimpleNftExample/SimpleNftExample.h"
#include "Multisig/Multisig.h"

int main() {
    AptosSDKDemo::AptosToken aptosToken;
    aptosToken.Start();
    AptosSDKDemo::TransferCoinExample transferCoinExample;
    transferCoinExample.Start();
    AptosSDKDemo::SimulateTransferCoin simulateTransferCoin;
    simulateTransferCoin.Start();
    AptosSDKDemo::SimpleNftExample simpleNftExample;
    simpleNftExample.Start();
    AptosSDKDemo::Multisig multisig;
    multisig.Start();

    return 0;
}
