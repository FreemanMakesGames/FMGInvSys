// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FMGInvSysItemCore.generated.h"

class AFMGInvSysItem;
class UFMGInvSysItemClicker;

/**
 * Because item core may be duplicated sometimes,
 * It's best to not give it any pointer member var that can differ between instances.
 */
UCLASS( Blueprintable, BlueprintType )
class FMGINVSYS_API UFMGInvSysItemCore : public UObject
{
	GENERATED_BODY()

public:

	UFMGInvSysItemCore( const FObjectInitializer& ObjectInitializer );

public:

	virtual bool operator==( const UFMGInvSysItemCore& Other ) { return GetClass() == Other.GetClass(); }

	virtual bool IsSupportedForNetworking() const override { return true; }

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	TSubclassOf<AFMGInvSysItem> GetItemClass() { return ItemClass; }

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	TSubclassOf<UFMGInvSysItemClicker> GetItemClickerClass() { return ItemClickerClass; }

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	UTexture2D* GetIcon() { return Icon; }

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
	TSubclassOf<UFMGInvSysItemClicker> ItemClickerClass;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "FMGInvSys" )
	UTexture2D* Icon;

	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	TArray<FString> ItemUsages;

	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	TMap<TSubclassOf<UFMGInvSysItemCore>, int> DismantleResults;

public:

	UFUNCTION( BlueprintCallable )
	int GetCount() { return Count; }

	UFUNCTION( BlueprintCallable )
	void SetCount( int InCount ) { Count = InCount; }

	UFUNCTION( BlueprintCallable )
	void AddCount( int Addition ) { Count += Addition; }
	
protected:

	int Count = 1;

public:

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable , Category = "FMGInvSys" )
	AFMGInvSysItem* SpawnItem( UWorld* World, const FTransform& SpawnTransform );
	virtual AFMGInvSysItem* SpawnItem_Implementation( UWorld* World, const FTransform& SpawnTransform );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "FMGInvSys" )
	FText Describe();
	virtual FText Describe_Implementation();

};
