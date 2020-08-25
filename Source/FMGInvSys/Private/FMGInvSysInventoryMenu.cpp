// Fill out your copyright notice in the Description page of Project Settings.


#include "FMGInvSysInventoryMenu.h"

#include "FMGInvSysItemClicker.h"
#include "FMGInvSysItemUsageButton.h"
#include "FMGInvSysInventory.h"
#include "FMGInvSysInventoryOwner.h"
#include "FMGInvSysItemCore.h"

#include "Components/WrapBox.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UFMGInvSysInventoryMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ensureAlways( DefaultItemClickerClass );
	ensureAlways( ItemUsageButtonClass );

	Button_AddToCombination->OnClicked.AddDynamic( this, &UFMGInvSysInventoryMenu::HandleOnButtonAddToCombinationClicked );
	Button_RemoveFromCombination->OnClicked.AddDynamic( this, &UFMGInvSysInventoryMenu::HandleOnButtonRemoveFromCombinationClicked );
	Button_Combine->OnClicked.AddDynamic( this, &UFMGInvSysInventoryMenu::HandleOnButtonCombineClicked );
	Button_Hide->OnClicked.AddDynamic( this, &UFMGInvSysInventoryMenu::HandleOnButtonHideClicked );
}

void UFMGInvSysInventoryMenu::Setup( IFMGInvSysInventoryOwner* NewInventoryOwner )
{
	if ( InventoryOwner == NewInventoryOwner )
	{
		ensureAlwaysMsgf( false, TEXT( "Why is the menu set up for the same inventory owner again?" ) );
		return;
	}

	if ( InventoryOwner )
		InventoryOwner->GetInventory()->OnItemCoresUpdated.RemoveDynamic( this, &UFMGInvSysInventoryMenu::HandleOnInventoryUpdated );
	InventoryOwner = NewInventoryOwner;
	InventoryOwner->GetInventory()->OnItemCoresUpdated.AddDynamic( this, &UFMGInvSysInventoryMenu::HandleOnInventoryUpdated );

	// Clean up old display.
	ResetLatestClicked();
	WrapBox_Clickers->ClearChildren();
	WrapBox_Clickers_Combining->ClearChildren();

	for ( UFMGInvSysItemCore* ItemCore : InventoryOwner->GetInventory()->GetItemCores() )
	{
		AddNewItemClicker( ItemCore );
	}
}

void UFMGInvSysInventoryMenu::Show_Implementation()
{
	AddToViewport();

	ResetLatestClicked();
}

void UFMGInvSysInventoryMenu::Hide_Implementation()
{
	ResetLatestClicked();

	RemoveFromParent();
}

UFMGInvSysItemClicker* UFMGInvSysInventoryMenu::AddNewItemClicker_Implementation( UFMGInvSysItemCore* ItemCore )
{
	UFMGInvSysItemClicker* ItemClicker;
	if ( TSubclassOf<UFMGInvSysItemClicker> CustomClickerClass = ItemCore->GetItemClickerClass() )
	{
		ItemClicker = UFMGInvSysItemClicker::InitItemClicker( this, CustomClickerClass, ItemCore );
	}
	else
	{
		ItemClicker = UFMGInvSysItemClicker::InitItemClicker( this, DefaultItemClickerClass, ItemCore );
	}

	ItemClicker->OnButtonClicked.AddDynamic( this, &UFMGInvSysInventoryMenu::HandleOnItemClickerClicked );

	WrapBox_Clickers->AddChildToWrapBox( ItemClicker );

	ItemToClicker.Add( ItemCore, ItemClicker );

	return ItemClicker;
}

void UFMGInvSysInventoryMenu::DisplayItemMenu_Implementation( UFMGInvSysItemCore* ItemCore )
{
	ItemMenu->ClearChildren();

	for ( FString ItemUsage : ItemCore->GetItemUsages() )
	{
		if ( UFMGInvSysItemUsageButton** pItemUsageButton = AllItemUsagesToButtons.Find( ItemUsage ) )
		{
			ItemMenu->AddChildToVerticalBox( *pItemUsageButton );
		}
		// Lazy instantiation if not found in map
		else
		{
			UFMGInvSysItemUsageButton* ItemUsageButton = UFMGInvSysItemUsageButton::InitItemUsageButton(
				this, ItemUsageButtonClass, ItemUsage );

			ItemUsageButton->OnClickedExt.AddDynamic( this, &UFMGInvSysInventoryMenu::HandleOnItemUsageButtonClicked );

			ItemMenu->AddChildToVerticalBox( ItemUsageButton );
			
			AllItemUsagesToButtons.Add( ItemUsage, ItemUsageButton );
		}
	}
}

void UFMGInvSysInventoryMenu::RemoveItemClicker_Implementation( UFMGInvSysItemCore* ItemCore )
{
	UFMGInvSysItemClicker** pItemClicker = ItemToClicker.Find( ItemCore );

	if ( pItemClicker )
	{
		if ( LatestClicked == *pItemClicker )
		{
			ResetLatestClicked();
		}

		( *pItemClicker )->RemoveFromParent();

		ItemToClicker.Remove( ItemCore );
	}
	else
		ensureAlways( false );
}

void UFMGInvSysInventoryMenu::ResetLatestClicked_Implementation()
{
	TextBlock_Description->SetText( FText::GetEmpty() );

	ItemMenu->ClearChildren();

	Button_AddToCombination->SetIsEnabled( false );
	Button_RemoveFromCombination->SetIsEnabled( false );

	if ( LatestClicked )
	{
		LatestClicked->Unhighlight();

		LatestClicked = nullptr;
	}
}

void UFMGInvSysInventoryMenu::HandleOnInventoryUpdated( TArray<UFMGInvSysItemCore*> Added, TArray<UFMGInvSysItemCore*> Removed )
{
	for ( UFMGInvSysItemCore* ItemCore : Removed )
	{
		RemoveItemClicker( ItemCore );
	}

	for ( UFMGInvSysItemCore* ItemCore : Added )
	{
		UFMGInvSysItemClicker* NewItemClicker = AddNewItemClicker( ItemCore );

		NewItemClicker->HighlightForAddition();
	}
}

void UFMGInvSysInventoryMenu::HandleOnItemClickerClicked( UFMGInvSysItemClicker* Clicked )
{
	UFMGInvSysItemCore* Core = Clicked->GetItemCore();

	DisplayItemMenu( Core );

	TextBlock_Description->SetText( Core->Describe() );
	
	if ( LatestClicked )
	{
		LatestClicked->Unhighlight();
	}
	Clicked->HighlightForClicking();

	if ( WrapBox_Clickers->GetAllChildren().Contains( Clicked ) )
	{
		Button_AddToCombination->SetIsEnabled( true );
		Button_RemoveFromCombination->SetIsEnabled( false );
	}
	else // It must be in the combining WrapBox.
	{
		Button_AddToCombination->SetIsEnabled( false );
		Button_RemoveFromCombination->SetIsEnabled( true );
	}

	LatestClicked = Clicked;
}

/**
 * Be careful. Calling an item removal from the inventory here will cause
 * HandleOnItemRemoval to be invoked by the inventory.
 */
void UFMGInvSysInventoryMenu::HandleOnItemUsageButtonClicked( UFMGInvSysItemUsageButton* ItemUsageButton )
{
	if ( !LatestClicked )
	{
		ensureAlwaysMsgf( false, TEXT( "Item menu may not have been properly hidden after an item usage like Destroy." ) );
		return;
	}

	APawn* ControlledPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if ( IFMGInvSysInventoryOwner* inventoryOwner = Cast<IFMGInvSysInventoryOwner>( ControlledPawn ) )
	{
		inventoryOwner->ApplyItemUsage( LatestClicked->GetItemCore(), ItemUsageButton->GetItemUsage() );
	}
	else
	{
		ensureAlwaysMsgf( false, TEXT( "If the player controller is controlling a non inventory owner, then inventory menu shouldn't be able to be opened." ) );
		return;
	}
}

void UFMGInvSysInventoryMenu::HandleOnButtonAddToCombinationClicked()
{
	if ( LatestClicked )
	{
		WrapBox_Clickers->RemoveChild( LatestClicked );
		WrapBox_Clickers_Combining->AddChildToWrapBox( LatestClicked );
	}
	else
	{
		ensureAlwaysMsgf( false, TEXT( "If there's no latest clicked, why is this button not disabled?" ) );
		return;
	}

	ResetLatestClicked();
}

void UFMGInvSysInventoryMenu::HandleOnButtonRemoveFromCombinationClicked()
{
	if ( LatestClicked )
	{
		WrapBox_Clickers_Combining->RemoveChild( LatestClicked );
		WrapBox_Clickers->AddChildToWrapBox( LatestClicked );
	}
	else
	{
		ensureAlwaysMsgf( false, TEXT( "If there's no latest clicked, why is this button not disabled?" ) );
		return;
	}

	ResetLatestClicked();
}

// This is a plugin made by Freeman. freeman at freemanmakesgames.pro

void UFMGInvSysInventoryMenu::HandleOnButtonCombineClicked()
{
	// LatestClicked = nullptr;

	if ( WrapBox_Clickers_Combining->GetChildrenCount() >= 2 )
	{
		TArray<UFMGInvSysItemCore*> SourceItemCores;

		for ( UWidget* Widget : WrapBox_Clickers_Combining->GetAllChildren() )
		{
			UFMGInvSysItemClicker* ItemClicker = Cast<UFMGInvSysItemClicker>( Widget );

			SourceItemCores.Add( ItemClicker->GetItemCore() );
		}

		InventoryOwner->Server_CombineItems( SourceItemCores );
	}
}

void UFMGInvSysInventoryMenu::HandleOnButtonHideClicked()
{
	Hide();
}
