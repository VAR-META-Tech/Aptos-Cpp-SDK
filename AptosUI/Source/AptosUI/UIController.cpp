// Fill out your copyright notice in the Description page of Project Settings.

#include "UIController.h"
#include <iostream>

void *UUIController::m_controller = AptosUILogic_createUiController();

void UUIController::OnCreateWalletClicked(FString netWork, FString &mnemonic_key_return, bool &IsCreateOk)
{
    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnCreateWalletClicked"));
    std::cerr << "UUIController::OnCreateWalletClicked" << std::endl;
    // Convert FString to const char*
    std::string MyStdString(TCHAR_TO_UTF8(*netWork));
    const char *MyNetWork = MyStdString.c_str();

    AptosUILogic_setNetwork(m_controller, MyNetWork);

    if (AptosUILogic_createNewWallet(m_controller))
    {
        char *mnemonic_key = AptosUILogic_getMnemonicsKey(m_controller);
        mnemonic_key_return = mnemonic_key;
        std::cout << mnemonic_key << std::endl;
        AptosUILogic_deleteString(mnemonic_key);
        IsCreateOk = true;
    }
    else
    {
        IsCreateOk = false;
    }
}

void UUIController::OnImportWalletClicked()
{
}
