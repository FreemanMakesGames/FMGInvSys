// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemCombiner.generated.h"

class UItemCore;

/**
 * It's actually an array of FName, which are names of the classes.
 */
USTRUCT( BlueprintType )
struct FItemCoreClassArray
{
	GENERATED_BODY()

public:

	FItemCoreClassArray() {}

	FItemCoreClassArray( TArray<FName> InItemCoreClasses )
	{
		ItemCoreClasses = InItemCoreClasses;
	}

	UPROPERTY( BlueprintReadOnly )
	TArray<FName> ItemCoreClasses;

	// Equivalence means having the same elements, regardless of the order.
	bool operator==( const FItemCoreClassArray& Other ) const
	{
		for ( FName ItemCoreClass : ItemCoreClasses )
		{
			if ( !Other.ItemCoreClasses.Contains( ItemCoreClass ) ) { return false; }
		}

		for ( FName ItemCoreClass : Other.ItemCoreClasses )
		{
			if ( !ItemCoreClasses.Contains( ItemCoreClass ) ) { return false; }
		}

		return true;
	}
};

FORCEINLINE uint32 GetTypeHash( const FItemCoreClassArray& ArrayOfItemCoreClasses )
{
	return FCrc::MemCrc_DEPRECATED( &ArrayOfItemCoreClasses, sizeof( FItemCoreClassArray ) );
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
UCLASS( Blueprintable, BlueprintType )
class FMGINVSYS_API UItemCombiner : public UObject
{
	GENERATED_BODY()

public:

	typedef FItemCombinationResult( UItemCombiner::*FCombineFunction )( TArray<UItemCore*> );

public:

	UItemCombiner();

protected:

	TMap<FItemCoreClassArray, FCombineFunction> FunctionMap;

	//TMap<TSubclassOf<UItemCore>, FCombineFunction> FunctionMap_WildCards;

public:

	FItemCombinationResult CombineItems( TArray<UItemCore*> SourceItems, bool Directional );

protected:

	FItemCombinationResult MakeWhiteGem( TArray<UItemCore*> SourceItems );
	
};
