// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

// #include "ItemCombiner.h"
// #include "DropItemComponent.h"
// #include "BasicCharacter.h"
// #include "BasicPlayerController.h"
// #include "DevUtilities.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

void UInventory::BeginPlay()
{
	Super::BeginPlay();

// 	ItemCombiner = NewObject<UItemCombiner>();
// 	ItemCombiner->SetPlayerController( Cast<ABasicPlayerController>( GetWorld()->GetFirstPlayerController() ) );
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

	OnItemAdded.Broadcast( ItemToAdd );
}

void UInventory::RemoveItem( UItemCore* ItemToRemove )
{
	ItemCores.Remove( ItemToRemove );

	OnItemRemoved.Broadcast( ItemToRemove );
}

void UInventory::DropItem( UItemCore* ItemToDrop )
{
// 	if ( UDropItemComponent* DropItemComponent = GetOwner()->FindComponentByClass<UDropItemComponent>() )
// 	{
// 		DropItemComponent->DropItem( ItemToDrop );
// 	}
// 	else
// 	{
// 		ensureAlwaysMsgf( false, TEXT( "The owner of this UInventory doesn't have a UDropItemComponent! Aborted." ) );
// 		return;
// 	}
// 
// 	RemoveItem( ItemToDrop );
}
