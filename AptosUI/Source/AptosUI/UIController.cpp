// Fill out your copyright notice in the Description page of Project Settings.

#include "UIController.h"
#include "Components/ComboBoxString.h"
#include <iostream>

void *UUIController::m_controller = AptosUILogic_createUiController();
int curindex = 0;
// Saving data
void SaveDataToFile(FString SaveData)
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("SaveData.json");
    FFileHelper::SaveStringToFile(SaveData, *SavePath);
}
// Loading data
FString LoadDataFromFile()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("SaveData.json");
    FString LoadedData;
    FFileHelper::LoadFileToString(LoadedData, *SavePath);
    return LoadedData;
}
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

void UUIController::OnImportWalletClicked(class UWidget *TargetComboBox, FString mnemonic_key_import, FString &balance_return, bool &IsImportOk)
{
    std::string MyStdString(TCHAR_TO_UTF8(*mnemonic_key_import));
    const char *mnemonic_key = MyStdString.c_str();
    if (AptosUILogic_restoreWallet(m_controller, mnemonic_key))
    {
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
            IsImportOk = false;
        }
        // std::cout << "Address list:" << std::endl;

        AptosUILogic_deleteStringArray(addressList, size);
        AptosUILogic_deleteString(balance);
        IsImportOk = true;
    }
    else
    {
        IsImportOk = false;
    }
}

void UUIController::OnWalletListDropdownValueChanged(int index, FString &balance_return)
{
    curindex = index;
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
    FString mnemonicUI = mnemonic;
    // TODO copy to clipboard
    FPlatformMisc::ClipboardCopy(*mnemonicUI);
    AptosUILogic_deleteString(mnemonic);
}

void UUIController::CopyPrivateKey()
{
    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnCopyPrivateKeyClicked"));
    // char *privateKey = AptosUILogic_onWalletListDropdownValueChanged(m_controller, curindex);
    char *privateKey = AptosUILogic_getPrivateKey(m_controller);
    FString privateKeyUI = privateKey;
    // TODO copy to clipboard
    FPlatformMisc::ClipboardCopy(*privateKeyUI);
    AptosUILogic_deleteString(privateKey);
}

void UUIController::Airdrop(FString &balance_return, bool &IsAirdropOk, FString &Notification)
{
    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnAirdropClicked"));
    int _amount = 100000000;
    AptosUILogic_airdrop(m_controller, 100000000);
    char *balance = AptosUILogic_getCurrentWalletBalanceText(m_controller);
    balance_return = balance;
    Notification = FString::Printf(TEXT("Successfully Get Airdrop of %d APT"), 1);
    IsAirdropOk = true;

    AptosUILogic_deleteString(balance);
}

void UUIController::Logout()
{
    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnLogoutClicked"));
    AptosUILogic_deleteUiController(m_controller);
    m_controller = AptosUILogic_createUiController();
}

void UUIController::SendToken(FString targetAddress, float amount, bool &IsSendTokenOk, FString &Notification, FString &balance_return)
{
    std::string _targetAddress(TCHAR_TO_UTF8(*targetAddress));
    long _amount = (long)(amount * 100000000);
    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnSendTokenClicked"));
    if (targetAddress.IsEmpty())
    {
        // notification
        IsSendTokenOk = false;
        Notification = FString::Printf(TEXT("Please Fill Out All Required Fields"));
    }
    else
    {
        bool success = AptosUILogic_sendToken(m_controller, _targetAddress.c_str(), _amount);
        if (success)
        {
            // notification
            IsSendTokenOk = true;
            Notification = FString::Printf(TEXT("Successfully send %f APT to %s...%s "), amount, *targetAddress.Left(10), *targetAddress.Right(10));
        }
        else
        {
            // notification
            IsSendTokenOk = false;
            Notification = FString::Printf(TEXT("Send Token Transaction Failed"));
        }
    }
    char *balance = AptosUILogic_getCurrentWalletBalanceText(m_controller);
    balance_return = balance;
    AptosUILogic_deleteString(balance);
}

void UUIController::CreateCollection(FString collectionName, FString collectionDescription, FString collectionUrl, bool &IsCreateCollectionOk, FString &Notification)
{
    std::string _collectionName(TCHAR_TO_UTF8(*collectionName));
    std::string _collectionDescription(TCHAR_TO_UTF8(*collectionDescription));
    std::string _collectionUrl(TCHAR_TO_UTF8(*collectionUrl));
    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnCreateCollectionClicked"));
    if (collectionName.IsEmpty() || collectionDescription.IsEmpty() || collectionUrl.IsEmpty())
    {
        // notification
        IsCreateCollectionOk = false;
        Notification = FString::Printf(TEXT("Please Fill Out All Required Fields"));
    }
    else
    {

        bool success = AptosUILogic_createCollection(m_controller, _collectionName.c_str(), _collectionDescription.c_str(), _collectionUrl.c_str());
        if (success)
        {
            // notification
            IsCreateCollectionOk = true;
            Notification = FString::Printf(TEXT("Successfully Create Collection: %s "), *collectionName);
        }
        else
        {
            // notification
            IsCreateCollectionOk = false;
            Notification = FString::Printf(TEXT("Failed to Create Collection: %s "), *collectionName);
        }
    }
}

void UUIController::CreateNFT(FString collectionName, FString tokenName, FString tokenDescription, int supply, int max, FString url, int royaltyPointsPerMillion, bool &IsCreateNFTOk, FString &Notification)
{

    UE_LOG(LogTemp, Warning, TEXT("UUIController::OnCreateNFTClicked"));
    std::string _collectionName(TCHAR_TO_UTF8(*collectionName));
    std::string _tokenName(TCHAR_TO_UTF8(*tokenName));
    std::string _tokenDescription(TCHAR_TO_UTF8(*tokenDescription));
    int _supply = supply;
    int _max = max;
    std::string _url(TCHAR_TO_UTF8(*url));
    int _royaltyPointsPerMillion = royaltyPointsPerMillion;
    if (collectionName.IsEmpty() || tokenName.IsEmpty() || tokenDescription.IsEmpty())
    {
        // notification
        IsCreateNFTOk = false;
        Notification = FString::Printf(TEXT("Please Fill Out All Required Fields"));
    }
    else
    {
        bool success = AptosUILogic_createNFT(m_controller, _collectionName.c_str(), _tokenName.c_str(), _tokenDescription.c_str(), _supply, _max, _url.c_str(), _royaltyPointsPerMillion);
        if (success)
        {
            // notification
            IsCreateNFTOk = true;
            Notification = FString::Printf(TEXT("Successfully Create NFT: %s "), *tokenName);
        }
        else
        {
            // notification
            IsCreateNFTOk = false;
            Notification = FString::Printf(TEXT("Failed to Create NFT: %s "), *tokenName);
        }
    }
}

void UUIController::OnInitApp(FString &mnemonic_key)
{
    mnemonic_key = LoadDataFromFile();
    if (!mnemonic_key.IsEmpty())
    {
        std::string _collectionName(TCHAR_TO_UTF8(*mnemonic_key));
        AptosUILogic_setNetwork(m_controller, "Devnet");
    }
}

void UUIController::OnQuitApp()
{
    char *mnemonic = AptosUILogic_getMnemonicsKey(m_controller);
    FString mnemonic_key = mnemonic;
    SaveDataToFile(mnemonic_key);
    AptosUILogic_deleteString(mnemonic);
}
