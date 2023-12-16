#ifndef UICONTROLLERLOGIC_H
#define UICONTROLLERLOGIC_H

#include <string>

extern "C" {
void* AptosUILogic_createUiController();
void AptosUILogic_deleteUiController(void* controller);
void AptosUILogic_setNetwork(void* controller, const char *target);
bool AptosUILogic_createNewWallet(void* controller);
bool AptosUILogic_initWalletFromCache(void* controller, const char* mnemonicsKey, int currentAddressIndexKey);
bool AptosUILogic_restoreWallet(void* controller, const char* mnemonicsKey);

char* AptosUILogic_getMnemonicsKey(void* controller);
char* AptosUILogic_getCurrentWalletBalanceText(void* controller);
void AptosUILogic_deleteString(char* str);
void AptosUILogic_deleteStringArray(char** strArr, int size);
char** AptosUILogic_getWalletAddress(void* controller, size_t *size);
char* AptosUILogic_onWalletListDropdownValueChanged(void* controller, int listIndex);
char* AptosUILogic_getCurrentWalletAddress(void* controller);
}

#endif // UICONTROLLERLOGIC_H
