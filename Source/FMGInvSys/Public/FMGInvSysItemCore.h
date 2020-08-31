// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Count.h"
#include "UObject/NoExportTypes.h"
#include "FMGInvSysItemCore.generated.h"

class AFMGInvSysItem;
class UFMGInvSysItemClicker;

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnDataChanged );

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

	UPROPERTY( BlueprintAssignable, Category = "FMGInvSys" )
	FOnDataChanged OnDataChanged;

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	TSubclassOf<AFMGInvSysItem> GetItemClass() const { return ItemClass; }

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	TSubclassOf<UFMGInvSysItemClicker> GetItemClickerClass() const { return ItemClickerClass; }

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	UTexture2D* GetIcon() const { return Icon; }

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	TArray<FString> GetItemUsages() const { return ItemUsages; }

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	TMap<TSubclassOf<UFMGInvSysItemCore>, int> GetDismantleResults() const
	{
		ensureAlwaysMsgf( DismantleResults.Num() > 0, TEXT( "This item core's dismantle results aren't setup, but it's being dismantled?" ) );

		return DismantleResults;
	}

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
    bool IsStackable() const { return bStackable; }

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	int GetMaxPerStack() const { return MaxPerStack; }

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

	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	bool bStackable = false;

	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	int MaxPerStack = 1;

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	int GetCount() const { return Count; }

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void SetCount( int InCount )
	{
		Count = InCount;

		OnDataChanged.Broadcast();
	}

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void AddCount( int Addition ) { SetCount( Count + Addition ); }
	
protected:

	UPROPERTY( ReplicatedUsing = OnRep_AnyData )
	int Count = 1;

public:

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable , Category = "FMGInvSys" )
	AFMGInvSysItem* SpawnItem( UWorld* World, const FTransform& SpawnTransform );
	virtual AFMGInvSysItem* SpawnItem_Implementation( UWorld* World, const FTransform& SpawnTransform );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "FMGInvSys" )
	FText Describe();
	virtual FText Describe_Implementation();

protected:

	UFUNCTION()
	void OnRep_AnyData() { OnDataChanged.Broadcast(); }

};
