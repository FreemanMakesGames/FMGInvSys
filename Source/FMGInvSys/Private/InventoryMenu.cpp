// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenu.h"

#include "ItemClicker.h"
#include "ItemUsageButton.h"
#include "Inventory.h"
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

	Button_Hide->OnClicked.AddDynamic( this, &UInventoryMenu::HandleOnButtonHideClicked );
}

void UInventoryMenu::Setup( UInventory* InInventory )
{
	if ( Inventory ) { ensureAlwaysMsgf( false, TEXT( "The current design is one menu for one unique inventory, and no other." ) ); return; }

	Inventory = InInventory;

	Inventory->OnItemAdded.AddDynamic( this, &UInventoryMenu::HandleOnItemAdded );
	Inventory->OnItemRemoved.AddDynamic( this, &UInventoryMenu::HandleOnItemRemoved );

	ensureAlwaysMsgf( !WrapBox_ItemClickers->HasAnyChildren(), TEXT( "Somehow the wrap box already has children?" ) );

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
}

void UInventoryMenu::Hide()
{
	IsCombining = false;

	RemoveFromParent();
}

void UInventoryMenu::SetupItemMenu()
{
	EItemUsage AllItemUsages[] = { EItemUsage::Drop, EItemUsage::Destroy };

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

	WrapBox_ItemClickers->AddChildToWrapBox( ItemClicker );

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
		( *pItemClicker )->RemoveFromParent();
	}
	else
	{
		ensureAlways( false );
	}

	// FIXME: Does ItemToItemClicker need to remove the ItemClicker?
}

void UInventoryMenu::HandleOnItemClickerClicked( UItemClicker* Clicked )
{
	UItemCore* ItemCore = Clicked->GetItemCore();

	// TODO: InventoryMenu: Display item description, if not combining.

	if ( IsCombining )
	{

	}
	else
	{
		DisplayItemMenu( ItemCore );

		FirstItemForCombination = ItemCore;
	}

	LatestClicked = Clicked;
}

/**
 * Be careful. Calling an item removal from the inventory here will cause
 * HandleOnItemRemoval to be invoked by the inventory.
 */
void UInventoryMenu::HandleOnItemUsageButtonClicked( UItemUsageButton* ItemUsageButton )
{
	if ( !LatestClicked || !LatestClicked->GetItemCore() )
	{
		ensureAlwaysMsgf( false, TEXT( "Item usage buttons may not have been properly hidden after an item usage like Destroy." ) );
		return;
	}

	switch ( ItemUsageButton->GetItemUsage() )
	{
	case EItemUsage::Drop:

		Inventory->DropItem( LatestClicked->GetItemCore() );

		// TODO: This code duplicates below in Destroy.
		ItemMenu->ClearChildren();
		LatestClicked = nullptr;

		break;

	case EItemUsage::Destroy:

		Inventory->RemoveItem( LatestClicked->GetItemCore() );

		ItemMenu->ClearChildren();
		LatestClicked = nullptr;

		break;

	default:

		ensureAlways( false );

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
}

/**
 * This can be invoked without inventory menu in view.
 */
void UInventoryMenu::HandleOnItemRemoved( UItemCore* ItemRemoved )
{
	RemoveItemClicker( ItemRemoved );
}

