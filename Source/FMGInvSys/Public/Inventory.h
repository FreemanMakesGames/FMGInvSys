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

	UPROPERTY( BlueprintAssignable )
	FOnItemAdded OnItemAdded;

	UPROPERTY( BlueprintAssignable )
	FOnItemRemoved OnItemRemoved;

public:

	UFUNCTION( BlueprintCallable )
	TArray<UItemCore*> GetItemCores();

protected:

	UPROPERTY( VisibleInstanceOnly )
	TArray<UItemCore*> ItemCores;

public:

	int CountItems();

	UFUNCTION( BlueprintCallable )
	void AddItem( UItemCore* ItemToAdd );

	UFUNCTION( BlueprintCallable )
	void RemoveItem( UItemCore* ItemToRemove );
		
};
