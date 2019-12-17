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
	if ( !InInventory ) { ensureAlways( false ); return; }
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

	VerticalBox_ItemUsageButtons->ClearChildren();
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
	VerticalBox_ItemUsageButtons->ClearChildren();

	for ( EItemUsage ItemUsage : ItemCore->GetItemUsages() )
	{
		if ( UItemUsageButton** pItemUsageButton = AllItemUsagesToButtons.Find( ItemUsage ) )
		{
			VerticalBox_ItemUsageButtons->AddChildToVerticalBox( *pItemUsageButton );
		}
		else
		{
			UE_LOG( LogTemp, Error, TEXT( "UInventoryMenu found an EItemUsage from a UItemCore, that doesn't match any UItemUsageButton in the TMap." ) );
		}
	}
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

	//LastClicked = Clicked;
}

void UInventoryMenu::HandleOnItemUsageButtonClicked( UItemUsageButton* ItemUsageButton )
{

}

void UInventoryMenu::HandleOnButtonHideClicked()
{
	Hide();
}

void UInventoryMenu::HandleOnItemAdded( UItemCore* ItemAdded )
{
	UItemClicker* NewItemClicker = AddNewItemClicker( ItemAdded );
}

void UInventoryMenu::HandleOnItemRemoved( UItemCore* ItemRemoved )
{
	UItemClicker** pItemClicker = ItemToClicker.Find( ItemRemoved );

	if ( pItemClicker )
	{
		( *pItemClicker )->RemoveFromParent();
	}
	else
	{
		ensureAlwaysMsgf( false, TEXT( "UInventoryMenu hears an item removal event, but no UItemClicker matches the removed item!" ) );
	}

	// FIXME: Does ItemToItemClicker need to remove the ItemClicker?
}

