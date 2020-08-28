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
	if ( !bCustomButtonStyle )
	{
		UTexture2D* Icon = ItemCore->GetIcon();
		
		Clicker->WidgetStyle.Normal.SetResourceObject( Icon );
		Clicker->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;
		
		Clicker->WidgetStyle.Hovered.SetResourceObject( Icon );
		Clicker->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;
		
		Clicker->WidgetStyle.Pressed.SetResourceObject( Icon );
		Clicker->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
		
		Clicker->WidgetStyle.Disabled.SetResourceObject( Icon );
		Clicker->WidgetStyle.Disabled.DrawAs = ESlateBrushDrawType::Image;
	}
}

void UFMGInvSysItemClicker::HandleOnButtonClicked()
{
	OnButtonClicked.Broadcast( this );
}

