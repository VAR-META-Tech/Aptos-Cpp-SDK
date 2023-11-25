// Fill out your copyright notice in the Description page of Project Settings.

#include "Editcombobox.h"
#include "Components/ComboBoxString.h"
// void UEditcombobox::EditComboBox(UComboBoxString *ComboBox, const TArray<FString> &NewOptions)
// {
//     if (ComboBox)
//     {
//         // Clear existing options
//         ComboBox->ClearOptions();

//         // Add new options
//         for (const FString &Option : NewOptions)
//         {
//             ComboBox->AddOption(Option);
//         }
//     }
// }

void UEditcombobox::EditComboBoxFunction(UWidget *TargetComboBox)
{
    UComboBoxString *MyCombobox = Cast<UComboBoxString>(TargetComboBox);
    if (MyCombobox)
    {
        // Clear existing options
        MyCombobox->ClearOptions();
        MyCombobox->AddOption(TEXT("Hello, Unreal!"));
    }
}
