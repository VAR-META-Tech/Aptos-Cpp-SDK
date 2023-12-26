// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define BOOST_NO_CXX98_FUNCTION_BASE
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wdeprecated-builtins"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wenum-constexpr-conversion"

#ifdef check
#undef check
#endif
#ifdef verify
#undef verify
#endif

#include "AptosUILogic/uicontrollerlogic.h"
#pragma GCC diagnostic pop

#include <string>
#include "UIController.generated.h"

/**
 *
 */
UCLASS()
class APTOSUI_API UUIController : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnCreateWallet Clicked"), Category = "UIController")
	static void OnCreateWalletClicked(class UWidget *TargetComboBox, FString netWork, FString &balance_return, FString &mnemonic_key_return, bool &IsCreateOk);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnImportWallet Clicked"), Category = "UIController")
	static void OnImportWalletClicked(FString mnemonic_key_import, bool &IsImportOk);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "WalletListDropdownValueChanged"), Category = "UIController")
	static void OnWalletListDropdownValueChanged(int index, FString &balance_return);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnCopyMnemonicWords Clicked"), Category = "UIController")
	static void CopyMnemonicWords();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnCopyPrivateKey Clicked"), Category = "UIController")
	static void CopyPrivateKey();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnAirdrop Clicked"), Category = "UIController")
	static void Airdrop();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnLogout Clicked"), Category = "UIController")
	static void Logout();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "On SendTransaction Clicked"), Category = "UIController")
	static void SendToken();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnCreateCollection Clicked"), Category = "UIController")
	static void CreateCollection();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnCreateNFT Clicked"), Category = "UIController")
	static void CreateNFT();

private:
	static void *m_controller;
};