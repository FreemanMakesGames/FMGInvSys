// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "ItemCore.h"
#include "ItemWidget.h"

// Sets default values
AItem::AItem( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick = true;

	// Replicate?
}

UItemCore* AItem::GetItemCore()
{
	return ItemCore;
}

void AItem::SetItemCore( UItemCore* InItemCore )
{
	if ( !ItemCore )
	{
		ItemCore = InItemCore;
	}
	else
	{
		//UDevUtilities::PrintError( "This AItem already has a UItemInfo!" );
		return;
	}
}
