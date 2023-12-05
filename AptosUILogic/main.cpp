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
    AptosUILogic_deleteUiController(controller);
}
