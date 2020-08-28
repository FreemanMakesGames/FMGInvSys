// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMGInvSysInventory.generated.h"

class UFMGInvSysItemCore;
class AFMGInvSysItemCombiner;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnItemCoresUpdated, UFMGInvSysItemCore*, ChangedItemCore );

UCLASS( Blueprintable, BlueprintType, ClassGroup=( Custom ), meta=( BlueprintSpawnableComponent ) )
class FMGINVSYS_API UFMGInvSysInventory : public UActorComponent
{
	GENERATED_BODY()

public:

	UFMGInvSysInventory();

public:

	/**
	 * This is fired either in AddItem and RemoveItem, updating one item a time, for listen-server or standalone,
	 * Or in OnRep_ItemCores, updating in bulk ( though usually an item I guess ), for clients.
	 */
	UPROPERTY( BlueprintAssignable )
	FOnItemCoresUpdated OnItemCoresUpdated;

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	TArray<UFMGInvSysItemCore*> GetItemCores() { return ItemCores; }

protected:

	UPROPERTY( VisibleInstanceOnly, Category = "FMGInvSys" )
	TArray<UFMGInvSysItemCore*> ItemCores;

public:

	int CountItems() { return ItemCores.Num(); }

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void AddItem( UFMGInvSysItemCore* ItemToAdd );

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void RemoveItem( UFMGInvSysItemCore* ItemToRemove, int Amount );

protected:

	UPROPERTY( ReplicatedUsing = OnRep_ChangedItemCore )
	UFMGInvSysItemCore* ChangedItemCore;

protected:
	
	UFUNCTION()
	void OnRep_ChangedItemCore();

public:

	virtual bool ReplicateSubobjects( class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags ) override;
		
};
