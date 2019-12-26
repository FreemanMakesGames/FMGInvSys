// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemClicker.h"

#include "ItemCore.h"
#include "ItemWidget.h"

#include "Components/Button.h"
#include "Components/NamedSlot.h"

void UItemClicker::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Clicker->OnClicked.AddDynamic( this, &UItemClicker::HandleOnButtonClicked );
}

UItemCore* UItemClicker::GetItemCore()
{
	return ItemCore;
}

void UItemClicker::SetItemCore( UItemCore* InItemCore )
{
	ItemCore = InItemCore;

	// Check and clear Named Slot's content.
	if ( ItemWidgetSlot->HasAnyChildren() )
	{
		UE_LOG( LogTemp, Warning, TEXT( "An ItemClicker's ItemCore is being replaced. Is it intended?" ) );

		ItemWidgetSlot->ClearChildren();
	}

	UItemWidget* ItemWidget = CreateWidget<UItemWidget>( this, ItemCore->GetItemWidgetClass() );
	ItemWidget->SetItemCore( ItemCore );

	ItemWidgetSlot->AddChild( ItemWidget );
}

void UItemClicker::HandleOnButtonClicked()
{
	OnButtonClicked.Broadcast( this );
}

