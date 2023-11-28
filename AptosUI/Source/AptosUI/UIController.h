// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIController.generated.h"

/**
 *
 */
UCLASS()
class APTOSUI_API UUIController : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnCreateWallet Clicked"), Category = "UIController")
	static void OnCreateWalletClicked();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnImportWallet Clicked"), Category = "UIController")
	static void OnImportWalletClicked();
};
