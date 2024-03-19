#include <iostream>
#include "../LIB/uicontrollerlogic.h"

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

    AptosUILogic_restoreWallet(controller, "wine resemble off banana exile upon color borrow rookie make urge evoke");
    size_t size = 0;
    char** addressList = AptosUILogic_getWalletAddress(controller, &size);
    std::cout << "Address list:" << std::endl;
    for (std::size_t i = 0; i < size; ++i) {
        std::cout << addressList[i] << std::endl;
    }
    char* balance2 = AptosUILogic_getCurrentWalletBalanceText(controller);
    std::cout << balance2 << std::endl;
    AptosUILogic_deleteString(balance2);

    AptosUILogic_airdrop(controller, 100000000);
    char* balance3 = AptosUILogic_getCurrentWalletBalanceText(controller);
    std::cout << balance3 << std::endl;
    AptosUILogic_deleteString(balance3);

    std::cout << std::boolalpha << AptosUILogic_restoreWallet(controller, "wine evoke") << std::endl;

    AptosUILogic_deleteStringArray(addressList, size);
    AptosUILogic_deleteUiController(controller);
}
