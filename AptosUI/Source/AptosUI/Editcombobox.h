// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/ComboBoxString.h"
#include "Editcombobox.generated.h"

/**
 *
 */
UCLASS()
class APTOSUI_API UEditcombobox : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Edit String To Combobox"), Category = "ComboBoxFunctions")
	static void EditComboBoxFunction(class UWidget *TargetComboBox);
};
