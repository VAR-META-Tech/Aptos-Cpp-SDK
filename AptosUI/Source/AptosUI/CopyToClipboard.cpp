// Fill out your copyright notice in the Description page of Project Settings.

#include "CopyToClipboard.h"

void UCopyToClipboard::CopyStringToClipboard(FString text)
{
    FPlatformMisc::ClipboardCopy(*text);
}