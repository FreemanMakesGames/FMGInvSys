// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "ItemDrop.h"

void UInventory::BeginPlay()
{
	Super::BeginPlay();

// 	ItemCombiner = NewObject<UItemCombiner>();
// 	ItemCombiner->SetPlayerController( Cast<ABasicPlayerController>( GetWorld()->GetFirstPlayerController() ) );
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
