// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenu.h"

#include "ItemClicker.h"
#include "ItemMenu.h"
#include "Inventory.h"
#include "Item.h"
#include "ItemWidget.h"

#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "Components/NamedSlot.h"


void UInventoryMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if ( !ItemClickerClass )
	{
		UE_LOG( LogTemp, Error, TEXT( "InventoryMenu's ItemClickerClass isn't assigned!" ) );
		return;
	}

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

	if ( ItemMenu->IsInViewport() )
	{
		ItemMenu->RemoveFromParent();
	}

	RemoveFromParent();
}

UItemClicker* UInventoryMenu::AddNewItemClicker( UItemCore* ItemCore )
{
	UItemClicker* ItemClicker = CreateWidget<UItemClicker>( this, ItemClickerClass );

	ItemClicker->SetItemCore( ItemCore );

	ItemClicker->OnButtonClicked.AddDynamic( this, &UInventoryMenu::HandleOnItemClickerClicked );
	
	WrapBox_ItemClickers->AddChildWrapBox( ItemClicker );

	ItemToClicker.Add( ItemCore, ItemClicker );

	return ItemClicker;
}

void UInventoryMenu::HandleOnItemClickerClicked( UItemClicker* Clicked )
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

