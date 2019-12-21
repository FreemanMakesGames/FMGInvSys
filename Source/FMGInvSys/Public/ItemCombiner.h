// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemCombiner.generated.h"

class UItemCore;

USTRUCT()
struct FItemCoreClassSet
{
	GENERATED_BODY()

public:

	FItemCoreClassSet() {}

	FItemCoreClassSet( TSet<TSubclassOf<UItemCore>> InItemCoreClasses )
	{
		ItemCoreClasses = InItemCoreClasses;
	}

	TSet<TSubclassOf<UItemCore>> ItemCoreClasses;

	bool operator==( const FItemCoreClassSet& Other ) const
	{
		return ItemCoreClasses.Difference( Other.ItemCoreClasses ).Num() == 0 && Other.ItemCoreClasses.Difference( ItemCoreClasses ).Num() == 0;
	}

};

FORCEINLINE uint32 GetTypeHash( const FItemCoreClassSet& ItemCoreClassSet )
{
	return FCrc::MemCrc_DEPRECATED( &ItemCoreClassSet, sizeof( FItemCoreClassSet ) );
}


USTRUCT()
struct FItemCombinationResult
{
	GENERATED_BODY()

public:

	FItemCombinationResult() { Successful = false; }

	FItemCombinationResult( bool InSuccessful, TArray<UItemCore*> InResultItems )
	{
		Successful = InSuccessful;
		ResultItems = InResultItems;
	}

	bool Successful;

	TArray<UItemCore*> ResultItems;

};

/**
 * 
 */
UCLASS()
class FMGINVSYS_API UItemCombiner : public UObject
{
	GENERATED_BODY()

public:

	typedef FItemCombinationResult( UItemCombiner::*FCombineFunction )( TArray<UItemCore*> );

public:

	UItemCombiner();

protected:

	TMap<FItemCoreClassSet, FCombineFunction> FunctionMap;

	//TMap<TSubclassOf<UItemCore>, FCombineFunction> FunctionMap_WildCards;

public:

	FItemCombinationResult CombineItems( TArray<UItemCore*> SourceItems, bool Directional );
	
};
