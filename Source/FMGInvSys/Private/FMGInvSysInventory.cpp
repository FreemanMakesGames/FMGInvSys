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

	ItemCores.Add( ItemToAdd );

	// Because OnRep_ItemCores won't fire in these net modes
	if ( GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_Standalone )
	{
		TArray<UFMGInvSysItemCore*> Added; Added.Add( ItemToAdd ); TArray<UFMGInvSysItemCore*> Removed;
		OnItemCoresUpdated.Broadcast( Added, Removed );
	}
}

void UFMGInvSysInventory::RemoveItem( UFMGInvSysItemCore* ItemToRemove )
{
	ensureAlways( ItemToRemove );

	ensureAlways( GetNetMode() != ENetMode::NM_Client );

	ItemCores.Remove( ItemToRemove );

	// Because OnRep_ItemCores won't fire in these net modes
	if ( GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_Standalone )
	{
		TArray<UFMGInvSysItemCore*> Added; TArray<UFMGInvSysItemCore*> Removed; Removed.Add( ItemToRemove );
		OnItemCoresUpdated.Broadcast( Added, Removed );
	}
}

void UFMGInvSysInventory::OnRep_ItemCores()
{
	TArray<UFMGInvSysItemCore*> Added;
	TArray<UFMGInvSysItemCore*> Removed;

	for ( UFMGInvSysItemCore* ItemCore : ItemCores )
		if ( !LastItemCores.Contains( ItemCore ) )
			Added.Add( ItemCore );

	for ( UFMGInvSysItemCore* ItemCore : LastItemCores )
		if ( !ItemCores.Contains( ItemCore ) )
			Removed.Add( ItemCore );

	OnItemCoresUpdated.Broadcast( Added, Removed );

	LastItemCores = ItemCores;
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
