// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "ItemDrop.h"
#include "ItemCombiner.h"

void UInventory::BeginPlay()
{
	Super::BeginPlay();

	ItemCombiner = NewObject<UItemCombiner>();
}

TArray<UItemCore*> UInventory::GetItemCores()
{
	return ItemCores;
}

int UInventory::CountItems()
{
	return ItemCores.Num();
}

void UInventory::AddItem( UItemCore* ItemToAdd )
{
	ensureAlways( ItemToAdd );

	ItemCores.Add( ItemToAdd );

	OnItemAdded.Broadcast( ItemToAdd );
}

void UInventory::RemoveItem( UItemCore* ItemToRemove )
{
	ItemCores.Remove( ItemToRemove );

	OnItemRemoved.Broadcast( ItemToRemove );
}

void UInventory::DropItem( UItemCore* ItemToDrop )
{
	if ( UItemDrop* ItemDrop = GetOwner()->FindComponentByClass<UItemDrop>() )
	{
		ItemDrop->DropItem( ItemToDrop );
	}
	else
	{
		ensureAlways( false );
		return;
	}

	RemoveItem( ItemToDrop );
}

void UInventory::CombineItems( TArray<UItemCore*> SourceItems )
{
	FItemCombinationResult Result = ItemCombiner->CombineItems( SourceItems, false );

	if ( !Result.Successful ) { return; }

	for ( UItemCore* Item : SourceItems )
	{
		RemoveItem( Item );
	}

	for ( UItemCore* Item : Result.ResultItems )
	{
		AddItem( Item );
	}
}
