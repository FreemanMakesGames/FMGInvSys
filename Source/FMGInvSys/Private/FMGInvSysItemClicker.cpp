// Fill out your copyright notice in the Description page of Project Settings.


#include "FMGInvSysItemClicker.h"

#include "FMGInvSysItemCore.h"

#include "Components/Button.h"

void UFMGInvSysItemClicker::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Clicker->OnClicked.AddDynamic( this, &UFMGInvSysItemClicker::HandleOnButtonClicked );
}

void UFMGInvSysItemClicker::SetItemCore_Implementation( UFMGInvSysItemCore* InItemCore )
{
	ItemCore = InItemCore;

	// Give plugin user the convenience to have button image setup automatically.
	// But don't do it if they'll be using custom images for button's different states.
	if ( !bUseCustomIcons )
	{
		Clicker->WidgetStyle.Normal.SetResourceObject( ItemCore->GetIcon() );
		Clicker->WidgetStyle.Hovered.SetResourceObject( ItemCore->GetIcon() );
		Clicker->WidgetStyle.Pressed.SetResourceObject( ItemCore->GetIcon() );
		Clicker->WidgetStyle.Disabled.SetResourceObject( ItemCore->GetIcon() );
	}
}

void UFMGInvSysItemClicker::HandleOnButtonClicked()
{
	OnButtonClicked.Broadcast( this );
}

