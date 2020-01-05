// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenu.h"

#include "ItemClicker.h"
#include "ItemUsageButton.h"
#include "Inventory.h"
#include "InventoryOwner.h"
#include "ItemCore.h"
#include "ItemWidget.h"
#include "Item.h"

#include "Components/WrapBox.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/NamedSlot.h"

void UInventoryMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ensureAlways( ItemClickerClass );
	ensureAlways( ItemUsageButtonClass );

	SetupItemMenu();

	Button_AddToCombination->OnClicked.AddDynamic( this, &UInventoryMenu::HandleOnButtonAddToCombinationClicked );
	Button_RemoveFromCombination->OnClicked.AddDynamic( this, &UInventoryMenu::HandleOnButtonRemoveFromCombinationClicked );
	Button_Combine->OnClicked.AddDynamic( this, &UInventoryMenu::HandleOnButtonCombineClicked );
	Button_Hide->OnClicked.AddDynamic( this, &UInventoryMenu::HandleOnButtonHideClicked );
}

void UInventoryMenu::Setup( UInventory* InInventory )
{
	if ( Inventory ) { ensureAlwaysMsgf( false, TEXT( "The current design is one menu for one unique inventory, and no other." ) ); return; }

	Inventory = InInventory;

	Inventory->OnItemAdded.AddDynamic( this, &UInventoryMenu::HandleOnItemAdded );
	Inventory->OnItemRemoved.AddDynamic( this, &UInventoryMenu::HandleOnItemRemoved );

	ensureAlwaysMsgf( !WrapBox_Clickers->HasAnyChildren(), TEXT( "Somehow the wrap box already has children?" ) );

	TArray<UItemCore*> Items = Inventory->GetItemCores();

	for ( int i = 0; i < Items.Num(); i++ )
	{
		if ( ensureAlways( Items[i] ) )
		{
			AddNewItemClicker( Items[i] );
		}
	}
}

void UInventoryMenu::Show()
{
	AddToViewport();

	ResetLatestClicked();
}

void UInventoryMenu::Hide()
{
	ResetLatestClicked();

	RemoveFromParent();
}

void UInventoryMenu::SetupItemMenu()
{
	EItemUsage AllItemUsages[] = { EItemUsage::Equip, EItemUsage::Drop, EItemUsage::Destroy };

	// Create one button for each item usage. They should always be reused and never be destroyed.
	for ( EItemUsage ItemUsage : AllItemUsages )
	{
		UItemUsageButton* ItemUsageButton = InitItemUsageButton( ItemUsage );

		ItemUsageButton->OnClickedExt.AddDynamic( this, &UInventoryMenu::HandleOnItemUsageButtonClicked );

		AllItemUsagesToButtons.Add( ItemUsage, ItemUsageButton );
	}

	ItemMenu->ClearChildren();
}

UItemUsageButton* UInventoryMenu::InitItemUsageButton( EItemUsage ItemUsage )
{
	UItemUsageButton* ItemUsageButton = CreateWidget<UItemUsageButton>( this, ItemUsageButtonClass );

	ItemUsageButton->SetItemUsage( ItemUsage );

	return ItemUsageButton;
}

UItemClicker* UInventoryMenu::AddNewItemClicker( UItemCore* ItemCore )
{
	UItemClicker* ItemClicker = CreateWidget<UItemClicker>( this, ItemClickerClass );

	ItemClicker->SetItemCore( ItemCore );

	ItemClicker->OnButtonClicked.AddDynamic( this, &UInventoryMenu::HandleOnItemClickerClicked );

	WrapBox_Clickers->AddChildToWrapBox( ItemClicker );

	ItemToClicker.Add( ItemCore, ItemClicker );

	return ItemClicker;
}

void UInventoryMenu::DisplayItemMenu( UItemCore* ItemCore )
{
	ItemMenu->ClearChildren();

	for ( EItemUsage ItemUsage : ItemCore->GetItemUsages() )
	{
		if ( UItemUsageButton** pItemUsageButton = AllItemUsagesToButtons.Find( ItemUsage ) )
		{
			ItemMenu->AddChildToVerticalBox( *pItemUsageButton );
		}
		else
		{
			UE_LOG( LogTemp, Error, TEXT( "UInventoryMenu found an EItemUsage from a UItemCore, that doesn't match any UItemUsageButton in the TMap." ) );
		}
	}
}

void UInventoryMenu::RemoveItemClicker( UItemCore* ItemCore )
{
	UItemClicker** pItemClicker = ItemToClicker.Find( ItemCore );

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
	{
		ensureAlways( false );
	}
}

void UInventoryMenu::ResetLatestClicked()
{
	// TODO: FMGInvSys: Clear description.

	ItemMenu->ClearChildren();

	Button_AddToCombination->SetIsEnabled( false );
	Button_RemoveFromCombination->SetIsEnabled( false );

	if ( LatestClicked )
	{
		LatestClicked->Unhighlight();

		LatestClicked = nullptr;
	}
}

void UInventoryMenu::HandleOnItemClickerClicked( UItemClicker* Clicked )
{
	UItemCore* ItemCore = Clicked->GetItemCore();

	DisplayItemMenu( ItemCore );
	
	if ( LatestClicked )
	{
		LatestClicked->Unhighlight();
	}
	Clicked->StopAllAnimations();
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
void UInventoryMenu::HandleOnItemUsageButtonClicked( UItemUsageButton* ItemUsageButton )
{
	if ( !LatestClicked )
	{
		ensureAlwaysMsgf( false, TEXT( "Item menu may not have been properly hidden after an item usage like Destroy." ) );
		return;
	}

	APawn* ControlledPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if ( IInventoryOwner* InventoryOwner = Cast<IInventoryOwner>( ControlledPawn ) )
	{
		InventoryOwner->Execute_ApplyItemUsage( ControlledPawn, LatestClicked->GetItemCore(), ItemUsageButton->GetItemUsage() );
	}
	else
	{
		ensureAlwaysMsgf( false, TEXT( "If the player controller is controlling a non inventory owner, then inventory menu shouldn't be able to be opened." ) );
		return;
	}
}

void UInventoryMenu::HandleOnButtonAddToCombinationClicked()
{
	if ( LatestClicked )
	{
		WrapBox_Clickers->RemoveChild( LatestClicked );
		WrapBox_Clickers_Combining->AddChildToWrapBox( LatestClicked );
	}

	ResetLatestClicked();
}

void UInventoryMenu::HandleOnButtonRemoveFromCombinationClicked()
{
	if ( LatestClicked )
	{
		WrapBox_Clickers_Combining->RemoveChild( LatestClicked );
		WrapBox_Clickers->AddChildToWrapBox( LatestClicked );
	}

	ResetLatestClicked();
}

void UInventoryMenu::HandleOnButtonCombineClicked()
{
	// LatestClicked = nullptr;

	if ( WrapBox_Clickers_Combining->GetChildrenCount() >= 2 )
	{
		TArray<UItemCore*> SourceItems;

		for ( UWidget* Widget : WrapBox_Clickers_Combining->GetAllChildren() )
		{
			UItemClicker* ItemClicker = Cast<UItemClicker>( Widget );

			if ( !ItemClicker ) { ensureAlways( false ); return; }

			SourceItems.Add( ItemClicker->GetItemCore() );
		}

		Inventory->CombineItems( SourceItems );
	}
}

void UInventoryMenu::HandleOnButtonHideClicked()
{
	Hide();
}

/**
 * This can be invoked without inventory menu in view.
 */
void UInventoryMenu::HandleOnItemAdded( UItemCore* ItemAdded )
{
	UItemClicker* NewItemClicker = AddNewItemClicker( ItemAdded );

	NewItemClicker->HighlightForAddition();
}

/**
 * This can be invoked without inventory menu in view.
 */
void UInventoryMenu::HandleOnItemRemoved( UItemCore* ItemRemoved )
{
	RemoveItemClicker( ItemRemoved );
}

