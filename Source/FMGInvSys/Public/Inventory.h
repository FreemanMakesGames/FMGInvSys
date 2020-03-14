// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemUsage.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

class UItemCore;
class AItemCombiner;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnItemAdded, UItemCore*, ItemCore );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnItemRemoved, UItemCore*, ItemCore );

UCLASS( Blueprintable, BlueprintType, ClassGroup=( Custom ), meta=( BlueprintSpawnableComponent ) )
class FMGINVSYS_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventory();

public:

	UPROPERTY( BlueprintAssignable )
	FOnItemAdded OnItemAdded;

	UPROPERTY( BlueprintAssignable )
	FOnItemRemoved OnItemRemoved;

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	TArray<UItemCore*> GetItemCores();

protected:

	UPROPERTY( Replicated, VisibleInstanceOnly, Category = "FMGInvSys" )
	TArray<UItemCore*> ItemCores;

public:

	int CountItems();

	UFUNCTION( NetMulticast, Reliable, BlueprintCallable, Category = "FMGInvSys" )
	void Multicast_AddItem( UItemCore* ItemToAdd );

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void RemoveItem( UItemCore* ItemToRemove );

// public:
// 
// 	virtual bool ReplicateSubobjects( class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags ) override;
		
};
