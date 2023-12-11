#include <iostream>
#include "uicontrollerlogic.h"

int main() {
    void* controller = AptosUILogic_createUiController();
    AptosUILogic_setNetwork(controller, "Devnet");
    AptosUILogic_createNewWallet(controller);
    char* mnemonic_key = AptosUILogic_getMnemonicsKey(controller);
    std::cout << mnemonic_key << std::endl;
    char* balance = AptosUILogic_getCurrentWalletBalanceText(controller);
    std::cout << balance << std::endl;
    AptosUILogic_deleteString(mnemonic_key);
    AptosUILogic_deleteString(balance);

    AptosUILogic_restoreWallet(controller, "carbon raccoon garlic phrase suspect brisk general credit excite meadow nerve soul");
    size_t size = 0;
    char** addressList = AptosUILogic_getWalletAddress(controller, &size);
    std::cout << "Address list:" << std::endl;
    for (std::size_t i = 0; i < size; ++i) {
        std::cout << addressList[i] << std::endl;
    }
    AptosUILogic_deleteStringArray(addressList, size);
    AptosUILogic_deleteUiController(controller);
}
