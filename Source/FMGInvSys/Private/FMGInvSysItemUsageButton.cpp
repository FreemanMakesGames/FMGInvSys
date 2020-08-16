// Fill out your copyright notice in the Description page of Project Settings.


#include "FMGInvSysItemUsageButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UFMGInvSysItemUsageButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_ItemUsage->OnClicked.AddDynamic( this, &UFMGInvSysItemUsageButton::HandleOnClicked );
}

EFMGInvSysItemUsage UFMGInvSysItemUsageButton::GetItemUsage()
{
	return ItemUsage;
}

void UFMGInvSysItemUsageButton::SetItemUsage( EFMGInvSysItemUsage InItemUsage )
{
	ItemUsage = InItemUsage;

	FText ItemUsageName = StaticEnum<EFMGInvSysItemUsage>()->GetDisplayNameTextByIndex( ( int32 )ItemUsage );
	TextBlock_ItemUsage->SetText( ItemUsageName );
}

void UFMGInvSysItemUsageButton::HandleOnClicked()
{
	OnClickedExt.Broadcast( this );
}
