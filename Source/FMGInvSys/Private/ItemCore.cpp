// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemCore.h"

#include "Item.h"
#include "ItemWidget.h"

TSubclassOf<AItem> UItemCore::GetItemClass()
{
	return ItemClass;
}

TSubclassOf<UItemWidget> UItemCore::GetItemWidgetClass()
{
	return ItemWidgetClass;
}

int UItemCore::GetItemTypeId()
{
	return ItemTypeId;
}

TArray<EItemUsage> UItemCore::GetItemUsages()
{
	return ItemUsages;
}

AItem* UItemCore::SpawnItem( const FTransform& SpawnTransform )
{
	AItem* Item = nullptr;

	if ( UWorld* World = GetWorld() )
	{
		Item = World->SpawnActor<AItem>( ItemClass, SpawnTransform );

		//this->Rename( nullptr, Item );

		//Item->SetItemInfo( this );
	}
	else
	{
		//UDevUtilities::PrintError( "This UItemInfo doesn't know the world!" );
	}

	return Item;
}
