// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemUsage.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

class UItemCore;
class AItemCombiner;

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnItemCoresUpdated );

UCLASS( Blueprintable, BlueprintType, ClassGroup=( Custom ), meta=( BlueprintSpawnableComponent ) )
class FMGINVSYS_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventory();

public:

	UPROPERTY( BlueprintAssignable )
	FOnItemCoresUpdated OnItemCoresUpdated;

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	TArray<UItemCore*> GetItemCores();

protected:

	UPROPERTY( ReplicatedUsing=OnRep_ItemCores, VisibleInstanceOnly, Category = "FMGInvSys" )
	TArray<UItemCore*> ItemCores;

public:

	int CountItems();

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void AddItem( UItemCore* ItemToAdd );

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void RemoveItem( UItemCore* ItemToRemove );

protected:

	UFUNCTION()
	void OnRep_ItemCores();

public:

	virtual bool ReplicateSubobjects( class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags ) override;
		
};
