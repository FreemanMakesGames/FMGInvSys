// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemUsageButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UItemUsageButton::NativeOnInitialized()
{
	Button_ItemUsage->OnClicked.AddDynamic( this, &UItemUsageButton::HandleOnClicked );
}

EItemUsage UItemUsageButton::GetItemUsage()
{
	return ItemUsage;
}

void UItemUsageButton::SetItemUsage( EItemUsage InItemUsage )
{
	ItemUsage = InItemUsage;

	FText ItemUsageName = StaticEnum<EItemUsage>()->GetDisplayNameTextByIndex( ( int32 )ItemUsage );
	TextBlock_ItemUsage->SetText( ItemUsageName );
}

void UItemUsageButton::HandleOnClicked()
{
	OnClickedExt.Broadcast( this );
}
