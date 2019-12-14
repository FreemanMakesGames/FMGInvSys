// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemUsageButton.h"

UItemUsageButton::UItemUsageButton( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	OnClicked.AddDynamic( this, &UItemUsageButton::HandleOnClicked );
}

EItemUsage UItemUsageButton::GetItemUsage()
{
	return ItemUsage;
}

void UItemUsageButton::SetItemUsage( EItemUsage InItemUsage )
{
	ItemUsage = InItemUsage;
}

void UItemUsageButton::HandleOnClicked()
{
	OnClickedExt.Broadcast( this );
}
