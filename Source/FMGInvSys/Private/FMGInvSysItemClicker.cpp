// Fill out your copyright notice in the Description page of Project Settings.


#include "FMGInvSysItemClicker.h"

#include "FMGInvSysItemCore.h"
#include "FMGInvSysItemWidget.h"

#include "Components/Button.h"
#include "Components/NamedSlot.h"

void UFMGInvSysItemClicker::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Clicker->OnClicked.AddDynamic( this, &UFMGInvSysItemClicker::HandleOnButtonClicked );
}

void UFMGInvSysItemClicker::SetItemCore( UFMGInvSysItemCore* InItemCore )
{
	ItemCore = InItemCore;

	// Check and clear Named Slot's content.
	if ( ItemWidgetSlot->HasAnyChildren() )
	{
		UE_LOG( LogTemp, Warning, TEXT( "An ItemClicker's ItemCore is being replaced. Is it intended?" ) );

		ItemWidgetSlot->ClearChildren();
	}

	UFMGInvSysItemWidget* ItemWidget = CreateWidget<UFMGInvSysItemWidget>( this, ItemCore->GetItemWidgetClass() );
	ItemWidget->SetItemCore( ItemCore );

	ItemWidgetSlot->AddChild( ItemWidget );
}

void UFMGInvSysItemClicker::HandleOnButtonClicked()
{
	OnButtonClicked.Broadcast( this );
}

