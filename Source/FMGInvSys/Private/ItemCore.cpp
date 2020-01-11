// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemCore.h"

#include "Item.h"
#include "ItemWidget.h"

UItemCore::UItemCore( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	// Add some common default EItemUsage, so the item designer doesn't have to add them manually for all kinds of UItemCore.
	ItemUsages.Add( EItemUsage::Drop );
	ItemUsages.Add( EItemUsage::Destroy );
}

AItem* UItemCore::SpawnItem( const FTransform& SpawnTransform )
{
	AItem* Item = nullptr;

	if ( UWorld* World = GetWorld() )
	{
		Item = World->SpawnActor<AItem>( ItemClass, SpawnTransform );

		//this->Rename( nullptr, Item );

		Item->SetItemCore( this );
	}
	else
	{
		ensureAlways( false );
	}

	return Item;
}

FText UItemCore::Describe_Implementation()
{
	//ensureAlways( false );
	return NSLOCTEXT( "", "", "Description is not overriden!" );
}
