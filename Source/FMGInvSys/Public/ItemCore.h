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
UCLASS( Blueprintable, BlueprintType )
class FMGINVSYS_API UItemCore : public UObject
{
	GENERATED_BODY()

public:

	UItemCore( const FObjectInitializer& ObjectInitializer );

public:

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AItem> GetItemClass() { return ItemClass; }

	UFUNCTION()
	TSubclassOf<UItemWidget> GetItemWidgetClass() { return ItemWidgetClass; }

	UFUNCTION( BlueprintCallable )
	TArray<EItemUsage> GetItemUsages() { return ItemUsages; }

protected:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TSubclassOf<AItem> ItemClass;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TSubclassOf<UItemWidget> ItemWidgetClass;

	UPROPERTY( EditDefaultsOnly )
	TArray<EItemUsage> ItemUsages;

public:

	UFUNCTION( BlueprintCallable )
	AItem* SpawnItem( const FTransform& SpawnTransform );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	FText Describe();

};
