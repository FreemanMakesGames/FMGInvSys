// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemUsage.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemCore.generated.h"

class AItem;
class UItemWidget;

/**
 * 
 */
UCLASS( BlueprintType )
class FMGINVSYS_API UItemCore : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AItem> GetItemClass();

	UFUNCTION()
	TSubclassOf<UItemWidget> GetItemWidgetClass();

	UFUNCTION( BlueprintCallable )
	int GetItemTypeId();

	UFUNCTION( BlueprintCallable )
	TArray<EItemUsage> GetItemUsages();

protected:

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TSubclassOf<AItem> ItemClass;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TSubclassOf<UItemWidget> ItemWidgetClass;

	int ItemTypeId;

	UPROPERTY()
	TArray<EItemUsage> ItemUsages;

public:

	UFUNCTION( BlueprintCallable )
	AItem* SpawnItem( const FTransform& SpawnTransform );

	UFUNCTION( BlueprintCallable )
	virtual UItemCore* Duplicate() PURE_VIRTUAL( , return nullptr; );

};