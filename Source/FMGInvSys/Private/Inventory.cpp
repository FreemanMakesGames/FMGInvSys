// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "ItemCore.h"
#include "InventoryOwner.h"
#include "ItemDrop.h"
#include "ItemCombiner.h"
#include "BasicGameMode.h"

void UInventory::BeginPlay()
{
	Super::BeginPlay();

	InventoryOwner = Cast<IInventoryOwner>( GetOwner() );
	if ( !InventoryOwner ) { ensureAlways( false ); }
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

void UInventory::ApplyItemUsage( UItemCore* ItemCore, EItemUsage ItemUsage )
{
	switch ( ItemUsage )
	{
	case EItemUsage::Destroy:

		RemoveItem( ItemCore );

		break;

	case EItemUsage::Drop:

		InventoryOwner->Execute_Drop( GetOwner(), ItemCore );

		RemoveItem( ItemCore );

		break;

	case EItemUsage::Equip:

		InventoryOwner->Execute_Equip( GetOwner(), ItemCore );

		EquippedItemCore = ItemCore;

		break;

	default:

		ensureAlways( false );

	}
}

void UInventory::CombineItems( TArray<UItemCore*> SourceItems )
{
	///////////////////

	// TODO FMGInvSys:

	// As the author I'm not creating one AItemCombiner per inventory, simply because it's unnecessary.
	// I provided an ABasicGameMode, which holds an AItemCombiner,
	// So it can be easily found and referenced from other classes, like the sample code below.

	// But this may contradict with your game, or you may want to do it in other ways.
	// You likely need to write your own code here to find the AItemCombiner in the game.

	// Note that you can have more than one AItemCombiner. For example,
	// The game mode can give a normal AItemCombiner most of the time,
	// And give a "blessed" AItemCombiner if the player has acquired a skill,
	// Or has entered a special area.

	AItemCombiner* ItemCombiner = GetWorld()->GetAuthGameMode<ABasicGameMode>()->GetItemCombiner();

	////////////////////////////////////////////////////////////////////////////////////////////////

	if ( !ItemCombiner ) { ensureAlways( false ); return; }

	FItemCombinationResult Result = ItemCombiner->CombineItems( SourceItems, false );

	if ( !Result.Successful ) { return; }

	for ( UItemCore* Item : SourceItems )
	{
		RemoveItem( Item );
	}

	for ( UItemCore* Item : Result.ResultItems )
	{
		AddItem( Item );
	}
}
