// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemMenu.h"

#include "Components/VerticalBox.h"

void UItemMenu::NativeOnInitialized()
{
	// Bind ItemUsageButton events.
	// ...

	VerticalBox_Buttons->ClearChildren();
}

void UItemMenu::Display( UItemCore* ItemCore )
{
	CurrentItemCore = ItemCore;

	VerticalBox_Buttons->ClearChildren();

	// Find ItemUsages from ItemCore and show corresponding ItemUsageButtons.
	// ...
}
