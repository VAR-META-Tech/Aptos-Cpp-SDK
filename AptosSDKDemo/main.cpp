#include <iostream>
#include "AptosToken/AptosToken.h"
#include "TransferCoin/TransferCoinExample.h"

int main() {
    AptosSDKDemo::AptosToken aptosToken;
    aptosToken.Start();
    AptosSDKDemo::TransferCoinExample transferCoinExample;
    transferCoinExample.Start();
    return 0;
}
