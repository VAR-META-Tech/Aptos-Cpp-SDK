// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CopyToClipboard.generated.h"

/**
 *
 */
UCLASS()
class APTOPSDEMO_API UCopyToClipboard : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Copy String To Clipboard"), Category = "AddStringToClipboard")
	static void CopyText(FString text);
};
