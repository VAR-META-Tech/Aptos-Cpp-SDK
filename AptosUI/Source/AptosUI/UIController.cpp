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

void UUIController::CopyMnemonicWords()
{
    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnCopyMnemonicWordsClicked"));
    char *mnemonic = AptosUILogic_getMnemonicsKey(m_controller);
    //TODO copy to clipboard
    AptosUILogic_deleteString(mnemonic);
}

void UUIController::CopyPrivateKey()
{
    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnCopyPrivateKeyClicked"));
    char *privateKey = AptosUILogic_getPrivateKey(m_controller);
    //TODO copy to clipboard
    AptosUILogic_deleteString(mnemonic);

}

void UUIController::Airdrop(int _amount)
{
    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnAirdropClicked"));
    AptosUILogic_airdrop(controller, _amount);
}

void UUIController::Logout()
{
    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnLogoutClicked"));
    AptosUILogic_deleteUiController(controller);
    m_controller = AptosUILogic_createUiController()
}

void UUIController::SendToken(string _targetAddress, long _amount)
{
    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnSendTokenClicked"));
    bool success = AptosUILogic_sendToken(controller, _targetAddress.c_str(), _amount)
    if (success) {
        //notification
    } else {
        //notification
    }
}

void UUIController::CreateCollection(string _collectionName, string _collectionDescription, string _collectionUri)
{
    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnCreateCollectionClicked"));
    bool success = AptosUILogic_createCollection(controller, _collectionName.c_str(), _collectionDescription.c_str(), _collectionUri.c_str())
    if (success) {
        //notification
    } else {
        //notification
    }
}

void UUIController::CreateNFT(string _collectionName, string _tokenName, string _tokenDescription, int _supply, int _max, string _uri, int _royaltyPointsPerMillion)
{
    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnCreateNFTClicked"));
        bool success = AptosUILogic_createNFT(_collectionName.c_str(), _tokenName.c_str(), _tokenDescription.c_str(), int _supply, int _max, _uri.c_str(), int _royaltyPointsPerMillion)
    if (success) {
        //notification
    } else {
        //notification
    }

}
