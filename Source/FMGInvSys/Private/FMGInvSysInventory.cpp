// Fill out your copyright notice in the Description page of Project Settings.


#include "FMGInvSysInventory.h"

#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

#include "FMGInvSysItemCore.h"
#include "FMGInvSysItemCombiner.h"

UFMGInvSysInventory::UFMGInvSysInventory()
{
	SetIsReplicatedByDefault( true );
}

void UFMGInvSysInventory::AddItem( UFMGInvSysItemCore* ItemToAdd )
{
	ensureAlways( ItemToAdd );

	ensureAlways( GetNetMode() != ENetMode::NM_Client );

	// Try to stack.
	// If it stacks, let item clicker's OnRep handle redrawing the display.
	if ( ItemToAdd->IsStackable() )
		for ( UFMGInvSysItemCore* ExistingCore : ItemCores )
		{
			if ( *ExistingCore == *ItemToAdd )
			{
				ExistingCore->AddCount( ItemToAdd->GetCount() );
				
				return;
			}
		}

	// If it doesn't stack
	
	ItemCores.Add( ItemToAdd );

	// Because OnRep_ItemCores won't fire in these net modes
	if ( GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_Standalone )
	{
		OnItemCoresUpdated.Broadcast( TArray<UFMGInvSysItemCore*>( &ItemToAdd, 1 ), TArray<UFMGInvSysItemCore*>() );
	}
}

void UFMGInvSysInventory::RemoveItem( UFMGInvSysItemCore* TargetItem, int Amount )
{
	ensureAlways( TargetItem );

	ensureAlways( GetNetMode() != ENetMode::NM_Client );

	ensureAlways( ItemCores.Contains( TargetItem ) );
	
	TargetItem->AddCount( -Amount );

	if ( TargetItem->GetCount() == 0 )
	{
		ItemCores.Remove( TargetItem );

		// Because OnRep_ItemCores won't fire in these net modes
		if ( GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_Standalone )
		{
			OnItemCoresUpdated.Broadcast( TArray<UFMGInvSysItemCore*>(), TArray<UFMGInvSysItemCore*>( &TargetItem, 1 ) );
		}
	}
	else if ( TargetItem->GetCount() < 0 )
		ensureAlways( false );

	// If item isn't depleted, just let item clicker's OnRep handle redrawing the display.
}

void UFMGInvSysInventory::OnRep_ItemCores()
{
	TArray<UFMGInvSysItemCore*> Added;
	TArray<UFMGInvSysItemCore*> Removed;

	for ( UFMGInvSysItemCore* ExistingCore : ItemCores )
		if ( !LastItemCores.Contains( ExistingCore ) )
			Added.Add( ExistingCore );

	for ( UFMGInvSysItemCore* ItemCore : LastItemCores )
		if ( !ItemCores.Contains( ItemCore ) )
			Removed.Add( ItemCore );

	LastItemCores = ItemCores;

	OnItemCoresUpdated.Broadcast( Added, Removed );

	// For items that are neither entirely new nor deleted,
	// Just let item clicker's OnRep handle redrawing the display.
}

bool UFMGInvSysInventory::ReplicateSubobjects( class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags )
{
	bool bWroteSomething = Super::ReplicateSubobjects( Channel, Bunch, RepFlags );

	for ( UFMGInvSysItemCore* ItemCore : ItemCores )
	{
		bWroteSomething |= Channel->ReplicateSubobject( ItemCore, *Bunch, *RepFlags );
	}

	return bWroteSomething;
}

void UFMGInvSysInventory::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );
	
	DOREPLIFETIME( UFMGInvSysInventory, ItemCores );
}
