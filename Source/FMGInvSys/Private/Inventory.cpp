// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "Net/UnrealNetwork.h"

#include "ItemCore.h"
#include "ItemDrop.h"
#include "ItemCombiner.h"
#include "BasicGameMode.h"

UInventory::UInventory()
{
	SetIsReplicated( true );
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

void UInventory::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( UInventory, ItemCores );
}
