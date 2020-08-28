// Fill out your copyright notice in the Description page of Project Settings.

#include "FMGInvSysItemCore.h"

#include "FMGInvSysItem.h"

UFMGInvSysItemCore::UFMGInvSysItemCore( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	// Add some common default EItemUsage, so the item designer doesn't have to add them manually for all kinds of UItemCore.
	ItemUsages.Add( "Drop" );
	ItemUsages.Add( "Destroy" );
}

AFMGInvSysItem* UFMGInvSysItemCore::SpawnItem( const FTransform& SpawnTransform )
{
	//ensureAlways( GetNetMode() != ENetMode::NM_Client );

	AFMGInvSysItem* Item = nullptr;

	if ( UWorld* World = GetWorld() )
	{
		Item = World->SpawnActor<AFMGInvSysItem>( ItemClass, SpawnTransform );

		SetOuterItem( Item );

		Item->SetItemCore( this );
	}
	else
		ensureAlways( false );

	return Item;
}

FText UFMGInvSysItemCore::Describe_Implementation()
{
	//ensureAlways( false );
	return NSLOCTEXT( "", "", "Description is not overriden!" );
}

void UFMGInvSysItemCore::SetOuterItem( AFMGInvSysItem* NewOuterItem )
{
	this->Rename( nullptr, NewOuterItem );

	// Trigger OnRep_OuterItem on client.
	OuterItem = NewOuterItem;
}

void UFMGInvSysItemCore::OnRep_OuterItem()
{
	this->Rename( nullptr, OuterItem );
}

void UFMGInvSysItemCore::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( UFMGInvSysItemCore, OuterItem );
}
