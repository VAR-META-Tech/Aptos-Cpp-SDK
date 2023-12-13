// Fill out your copyright notice in the Description page of Project Settings.

#include "UIController.h"
#include "Components/ComboBoxString.h"
#include <iostream>

void *UUIController::m_controller = AptosUILogic_createUiController();

void UUIController::OnCreateWalletClicked(class UWidget *TargetComboBox, FString netWork, FString &balance_return, FString &mnemonic_key_return, bool &IsCreateOk)
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

        char *balance = AptosUILogic_getCurrentWalletBalanceText(m_controller);
        balance_return = balance;
        std::cout << balance << std::endl;

        size_t size = 0;
        char **addressList = AptosUILogic_getWalletAddress(m_controller, &size);
        UComboBoxString *MyCombobox = Cast<UComboBoxString>(TargetComboBox);
        if (MyCombobox)
        {
            // Clear existing options
            MyCombobox->ClearOptions();
            for (std::size_t i = 0; i < size; ++i)
            {
                MyCombobox->AddOption(FString(addressList[i]));
                std::cout << addressList[i] << std::endl;
            }
            MyCombobox->SetSelectedIndex(0);
        }
        else
        {
            IsCreateOk = false;
        }
        // std::cout << "Address list:" << std::endl;

        AptosUILogic_deleteStringArray(addressList, size);
        AptosUILogic_deleteString(mnemonic_key);
        AptosUILogic_deleteString(balance);
        IsCreateOk = true;
    }
    else
    {
        IsCreateOk = false;
    }
}

void UUIController::OnImportWalletClicked(FString mnemonic_key_import, bool &IsImportOk)
{
    std::string MyStdString(TCHAR_TO_UTF8(*mnemonic_key_import));
    const char *mnemonic_key = MyStdString.c_str();
    if (AptosUILogic_restoreWallet(m_controller, mnemonic_key))
    {
        IsImportOk = true;
    }
    else
    {
        IsImportOk = false;
    }
}

void UUIController::OnWalletListDropdownValueChanged(int index, FString &balance_return)
{
    char *curAddress = AptosUILogic_onWalletListDropdownValueChanged(m_controller, index);
    char *balance = AptosUILogic_getCurrentWalletBalanceText(m_controller);
    balance_return = balance;
    AptosUILogic_deleteString(curAddress);
    AptosUILogic_deleteString(balance);
}