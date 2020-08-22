// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FMGInvSysItemCore.generated.h"

class AFMGInvSysItem;
class UFMGInvSysItemWidget;

/**
 * 
 */
UCLASS( Blueprintable, BlueprintType )
class FMGINVSYS_API UFMGInvSysItemCore : public UObject
{
	GENERATED_BODY()

public:

	UFMGInvSysItemCore( const FObjectInitializer& ObjectInitializer );

	virtual bool IsSupportedForNetworking() const override { return true; }

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	TSubclassOf<AFMGInvSysItem> GetItemClass() { return ItemClass; }

	UFUNCTION()
	TSubclassOf<UFMGInvSysItemWidget> GetItemWidgetClass() { return ItemWidgetClass; }

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	TArray<FString> GetItemUsages() { return ItemUsages; }

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	TMap<TSubclassOf<UFMGInvSysItemCore>, int> GetDismantleResults()
	{
		ensureAlwaysMsgf( DismantleResults.Num() > 0, TEXT( "This item core's dismantle results aren't setup, but it's being dismantled?" ) );

		return DismantleResults;
	}

protected:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "FMGInvSys" )
	TSubclassOf<AFMGInvSysItem> ItemClass;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "FMGInvSys" )
	TSubclassOf<UFMGInvSysItemWidget> ItemWidgetClass;

	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	TArray<FString> ItemUsages;

	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	TMap<TSubclassOf<UFMGInvSysItemCore>, int> DismantleResults;

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	AFMGInvSysItem* SpawnItem( const FTransform& SpawnTransform );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "FMGInvSys" )
	FText Describe();

};
