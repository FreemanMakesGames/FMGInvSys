// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

protected:

	virtual void BeginPlay() override;

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

	UPROPERTY( EditAnywhere )
	AItemCombiner* ItemCombiner;

public:

	UFUNCTION( BlueprintCallable )
	int CountItems();

	UFUNCTION( BlueprintCallable )
	void AddItem( UItemCore* ItemToAdd );

	UFUNCTION( BlueprintCallable )
	void RemoveItem( UItemCore* ItemToRemove );

	UFUNCTION( BlueprintCallable )
	void DropItem( UItemCore* ItemToDrop );

	UFUNCTION( BlueprintCallable )
	void CombineItems( TArray<UItemCore*> SourceItems );

		
};
