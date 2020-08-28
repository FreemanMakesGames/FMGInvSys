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

	bool bStacking = false;
	for ( UFMGInvSysItemCore* ExistingCore : ItemCores )
	{
		if ( *ExistingCore == *ItemToAdd )
		{
			ExistingCore->AddCount( ItemToAdd->GetCount() );

			ChangedItemCore = ExistingCore;
			
			bStacking = true;
			break;
		}
	}
	if ( !bStacking )
	{
		ItemCores.Add( ItemToAdd );

		ChangedItemCore = ItemToAdd;
	}

	// Because OnRep_ItemCores won't fire in these net modes
	if ( GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_Standalone )
	{
		OnItemCoresUpdated.Broadcast( ChangedItemCore );
	}
}

void UFMGInvSysInventory::RemoveItem( UFMGInvSysItemCore* TargetItem, int Amount )
{
	ensureAlways( TargetItem );

	ensureAlways( GetNetMode() != ENetMode::NM_Client );

	ensureAlways( ItemCores.Contains( TargetItem ) );
	
	TargetItem->AddCount( -Amount );

	if ( TargetItem->GetCount() == 0 )
		ItemCores.Remove( TargetItem );
	else if ( TargetItem->GetCount() < 0 )
		ensureAlways( false );

	ChangedItemCore = TargetItem;

	// Because OnRep_ItemCores won't fire in these net modes
	if ( GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_Standalone )
	{
		OnItemCoresUpdated.Broadcast( ChangedItemCore );
	}
}

void UFMGInvSysInventory::OnRep_ChangedItemCore()
{
	int idx;
	if ( ItemCores.Find( ChangedItemCore, idx ) )
	{
		ItemCores.RemoveAt( idx );
	}
	ItemCores.Add( ChangedItemCore );

	OnItemCoresUpdated.Broadcast( ChangedItemCore );
}

bool UFMGInvSysInventory::ReplicateSubobjects( class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags )
{
	bool bWroteSomething = Super::ReplicateSubobjects( Channel, Bunch, RepFlags );

	bWroteSomething |= Channel->ReplicateSubobject( ChangedItemCore, *Bunch, *RepFlags );

	return bWroteSomething;
}

void UFMGInvSysInventory::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( UFMGInvSysInventory, ChangedItemCore );
}
