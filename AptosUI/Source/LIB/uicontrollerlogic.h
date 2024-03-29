#ifndef UICONTROLLERLOGIC_H
#define UICONTROLLERLOGIC_H

#include <string>

#if defined(_WIN32) || defined(_WIN64)
    #if defined(BUILDING_APTOS_DLL)
    #define APTOS_API __declspec(dllexport)
    #else
    #define APTOS_API __declspec(dllimport)
    #endif
#else
    #define APTOS_API
#endif

extern "C" {
APTOS_API void* AptosUILogic_createUiController();
APTOS_API void AptosUILogic_deleteUiController(void* controller);
APTOS_API void AptosUILogic_setNetwork(void* controller, const char *target);
APTOS_API bool AptosUILogic_createNewWallet(void* controller);
APTOS_API bool AptosUILogic_initWalletFromCache(void* controller, const char* mnemonicsKey, int currentAddressIndexKey);
APTOS_API bool AptosUILogic_restoreWallet(void* controller, const char* mnemonicsKey);
APTOS_API char* AptosUILogic_getMnemonicsKey(void* controller);
APTOS_API char* AptosUILogic_getPrivateKey(void* controller);
APTOS_API char* AptosUILogic_getCurrentWalletBalanceText(void* controller);
APTOS_API void AptosUILogic_deleteString(char* str);
APTOS_API void AptosUILogic_deleteStringArray(char** strArr, int size);
APTOS_API char** AptosUILogic_getWalletAddress(void* controller, size_t *size);
APTOS_API char* AptosUILogic_onWalletListDropdownValueChanged(void* controller, int listIndex);
APTOS_API char* AptosUILogic_getCurrentWalletAddress(void* controller);
APTOS_API void AptosUILogic_airdrop(void* controller, int amount);
APTOS_API bool AptosUILogic_sendToken(void* controller, const char* _targetAddress, long _amount);
APTOS_API bool AptosUILogic_createCollection(void* controller, const char* _collectionName, const char* _collectionDescription, const char* _collectionUri);
APTOS_API bool AptosUILogic_createNFT(void* controller, const char* _collectionName, const char* _tokenName, const char* _tokenDescription, int _supply, int _max, const char* _uri, int _royaltyPointsPerMillion);

}

#endif // UICONTROLLERLOGIC_H
