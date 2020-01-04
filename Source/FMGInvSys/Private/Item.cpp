// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "ItemCore.h"
#include "ItemWidget.h"

AItem::AItem( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick = true;

}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	ensureAlways( ItemCoreClass );

	// This is for cases like when a level designer placed an AItem into the level manually.
	if ( !ItemCore )
	{
		ItemCore = NewObject<UItemCore>( this, ItemCoreClass );
	}
}

UItemCore* AItem::GetItemCore()
{
	return ItemCore;
}

void AItem::SetItemCore( UItemCore* InItemCore )
{
// 	if ( !ItemCore )
// 	{
// 		ItemCore = InItemCore;
// 	}
// 	else
// 	{
// 		ensureAlways( false );
// 		return;
// 	}

	ItemCore = InItemCore;
}
