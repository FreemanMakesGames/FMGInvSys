// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDrop.h"

#include "ItemCore.h"
#include "Item.h"

// Sets default values for this component's properties
UItemDrop::UItemDrop()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UItemDrop::DropItem( UItemCore* ItemCore )
{
	ItemCore->SpawnItem( GetComponentTransform() );
}
