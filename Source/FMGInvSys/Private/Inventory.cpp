// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "Engine/ActorChannel.h"
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

	if ( GetNetMode() == ENetMode::NM_ListenServer )
	{
		OnItemCoresUpdated.Broadcast();
	}
}

void UInventory::RemoveItem( UItemCore* ItemToRemove )
{
	ensureAlways( ItemToRemove );

	ItemCores.Remove( ItemToRemove );

	if ( GetNetMode() == ENetMode::NM_ListenServer )
	{
		OnItemCoresUpdated.Broadcast();
	}
}

void UInventory::OnRep_ItemCores()
{
	OnItemCoresUpdated.Broadcast();
}

bool UInventory::ReplicateSubobjects( class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags )
{
	bool bWroteSomething = Super::ReplicateSubobjects( Channel, Bunch, RepFlags );

	for ( UItemCore* ItemCore : ItemCores )
	{
		bWroteSomething |= Channel->ReplicateSubobject( ItemCore, *Bunch, *RepFlags );
	}

	return bWroteSomething;
}

void UInventory::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( UInventory, ItemCores );
}
