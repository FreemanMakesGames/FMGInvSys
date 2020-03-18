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

	ensureAlways( GetNetMode() != ENetMode::NM_Client );

	ItemCores.Add( ItemToAdd );

	// Because OnRep_ItemCores won't fire in these net modes
	if ( GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_Standalone )
	{
		TArray<UItemCore*> Added; Added.Add( ItemToAdd ); TArray<UItemCore*> Removed;
		OnItemCoresUpdated.Broadcast( Added, Removed );
	}
}

void UInventory::RemoveItem( UItemCore* ItemToRemove )
{
	ensureAlways( ItemToRemove );

	ensureAlways( GetNetMode() != ENetMode::NM_Client );

	ItemCores.Remove( ItemToRemove );

	// Because OnRep_ItemCores won't fire in these net modes
	if ( GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_Standalone )
	{
		TArray<UItemCore*> Added; TArray<UItemCore*> Removed; Removed.Add( ItemToRemove );
		OnItemCoresUpdated.Broadcast( Added, Removed );
	}
}

void UInventory::OnRep_ItemCores()
{
	TArray<UItemCore*> Added;
	TArray<UItemCore*> Removed;

	for ( UItemCore* ItemCore : ItemCores )
		if ( !LastItemCores.Contains( ItemCore ) )
			Added.Add( ItemCore );

	for ( UItemCore* ItemCore : LastItemCores )
		if ( !ItemCores.Contains( ItemCore ) )
			Removed.Add( ItemCore );

	OnItemCoresUpdated.Broadcast( Added, Removed );

	LastItemCores = ItemCores;
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
