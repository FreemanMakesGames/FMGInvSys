// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemMenu.h"

#include "ItemUsageButton.h"
#include "ItemCore.h"

#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"

UItemMenu::UItemMenu( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{

}

void UItemMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	EItemUsage AllItemUsages[] = { EItemUsage::Drop, EItemUsage::Destroy };

	// Create one button for each item usage. They should always be reused and never be destroyed.
	for ( EItemUsage ItemUsage : AllItemUsages )
	{
		UItemUsageButton* ItemUsageButton = InitItemUsageButton( ItemUsage );

		ItemUsageButton->OnClickedExt.AddDynamic( this, &UItemMenu::HandleOnItemUsageButtonClicked );

		AllItemUsagesToButtons.Add( ItemUsage, ItemUsageButton );
	}

	VerticalBox_Buttons->ClearChildren();
}

void UItemMenu::Display( UItemCore* ItemCore )
{
	CurrentItemCore = ItemCore;

	VerticalBox_Buttons->ClearChildren();

	for ( EItemUsage ItemUsage : ItemCore->GetItemUsages() )
	{
		if ( UItemUsageButton** pItemUsageButton = AllItemUsagesToButtons.Find( ItemUsage ) )
		{
			VerticalBox_Buttons->AddChildToVerticalBox( *pItemUsageButton );
		}
		else
		{
			UE_LOG( LogTemp, Error, TEXT( "UItemMenu found an EItemUsage from a UItemCore, that doesn't match any UItemUsageButton in the TMap." ) );
		}
	}
}

void UItemMenu::HandleOnItemUsageButtonClicked( UItemUsageButton* ItemUsageButton )
{
	OnItemUsageButtonClicked.Broadcast( ItemUsageButton->GetItemUsage(), CurrentItemCore );
}

UItemUsageButton* UItemMenu::InitItemUsageButton( EItemUsage ItemUsage )
{
	UItemUsageButton* ItemUsageButton = NewObject<UItemUsageButton>( this );

	ItemUsageButton->SetItemUsage( ItemUsage );

	return ItemUsageButton;
}
