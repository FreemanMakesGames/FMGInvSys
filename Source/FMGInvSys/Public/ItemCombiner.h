// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemCombiner.generated.h"

class UItemCore;

USTRUCT()
struct FArrayOfItemCoreClassArrays
{
	GENERATED_BODY()

public:

	FArrayOfItemCoreClassArrays() {}

	FArrayOfItemCoreClassArrays( TArray<TSubclassOf<UItemCore>> InItemCoreClasses )
	{
		ItemCoreClasses = InItemCoreClasses;
	}

	TArray<TSubclassOf<UItemCore>> ItemCoreClasses;

	bool operator==( const FArrayOfItemCoreClassArrays& Other ) const
	{
		return ItemCoreClasses == Other.ItemCoreClasses;
	}

};

FORCEINLINE uint32 GetTypeHash( const FArrayOfItemCoreClassArrays& ArrayOfItemCoreClasses )
{
	return FCrc::MemCrc_DEPRECATED( &ArrayOfItemCoreClasses, sizeof( FArrayOfItemCoreClassArrays ) );
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

	TMap<FArrayOfItemCoreClassArrays, FCombineFunction> FunctionMap;

	//TMap<TSubclassOf<UItemCore>, FCombineFunction> FunctionMap_WildCards;

public:

	FItemCombinationResult CombineItems( TArray<UItemCore*> SourceItems, bool Directional );
	
};
