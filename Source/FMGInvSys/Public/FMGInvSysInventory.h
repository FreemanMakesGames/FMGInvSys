// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMGInvSysInventory.generated.h"

class UFMGInvSysItemCore;
class AFMGInvSysItemCombiner;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnItemCoresUpdated, TArray<UFMGInvSysItemCore*>, Added,
	TArray<UFMGInvSysItemCore*>, Removed );

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
	TArray<UFMGInvSysItemCore*> GetItemCores() const { return ItemCores; }

protected:

	UPROPERTY( ReplicatedUsing = OnRep_ItemCores, VisibleInstanceOnly, Category = "FMGInvSys" )
	TArray<UFMGInvSysItemCore*> ItemCores;

	/** Used for checking new and deleted item cores after a replication. */
	TArray<UFMGInvSysItemCore*> LastItemCores;

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void AddItemCore( UFMGInvSysItemCore* ItemToAdd );

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void RemoveItemCore( UFMGInvSysItemCore* ItemToRemove, int Amount );

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	int CountItemStacks() const { return ItemCores.Num(); }

protected:

	UFUNCTION()
	void OnRep_ItemCores();

public:

	virtual bool ReplicateSubobjects( class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags ) override;
		
};
