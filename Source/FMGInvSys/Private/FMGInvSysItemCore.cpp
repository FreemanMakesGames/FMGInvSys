// Fill out your copyright notice in the Description page of Project Settings.

#include "FMGInvSysItemCore.h"

#include "FMGInvSysItem.h"

#include "Net/UnrealNetwork.h"

UFMGInvSysItemCore::UFMGInvSysItemCore( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	// Add some common default EItemUsage, so the item designer doesn't have to add them manually for all kinds of UItemCore.
	ItemUsages.Add( "Drop" );
	ItemUsages.Add( "Destroy" );
}

AFMGInvSysItem* UFMGInvSysItemCore::SpawnItem_Implementation( UWorld* World, const FTransform& SpawnTransform )
{
	//ensureAlways( GetNetMode() != ENetMode::NM_Client );

	AFMGInvSysItem* Item = World->SpawnActor<AFMGInvSysItem>( ItemClass, SpawnTransform );

	//this->Rename( nullptr, Item );

	Item->SetItemCore( this );

	return Item;
}

FText UFMGInvSysItemCore::Describe_Implementation()
{
	//ensureAlways( false );
	return NSLOCTEXT( "", "", "Description is not overriden!" );
}

void UFMGInvSysItemCore::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( UFMGInvSysItemCore, Count );
}
