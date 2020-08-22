// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Algo/Count.h"

#include "CoreMinimal.h"
#include "FMGInvSysItemCore.h"
#include "UObject/NoExportTypes.h"
#include "FMGInvSysItemCombiner.generated.h"

DECLARE_DELEGATE_RetVal_OneParam( FFMGInvSysCombineResult, FFMGInvSysCombineDelegate, TArray<UFMGInvSysItemCore*> );

class UFMGInvSysItemCore;

/**
 * It's actually an array of FName, which are names of the classes.
 */
USTRUCT( BlueprintType )
struct FFMGInvSysItemCoreClassArray
{
	GENERATED_BODY()

public:

	FFMGInvSysItemCoreClassArray() {}

	FFMGInvSysItemCoreClassArray( TArray<TSubclassOf<UFMGInvSysItemCore>> InItemCoreClasses )
	{
		ItemCoreClasses = InItemCoreClasses;
	}

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "FMGInvSys" )
	TArray<TSubclassOf<UFMGInvSysItemCore>> ItemCoreClasses;

	// Equivalence means having the same number of each element, regardless of the order.
	bool operator==( const FFMGInvSysItemCoreClassArray& Other ) const
	{
		for ( TSubclassOf<UFMGInvSysItemCore> ItemCoreClass : ItemCoreClasses )
		{
			if ( Algo::Count( ItemCoreClasses, ItemCoreClass ) != Algo::Count( Other.ItemCoreClasses, ItemCoreClass ) )
			{
				return false;
			}
		}

		for ( TSubclassOf<UFMGInvSysItemCore> ItemCoreClass : Other.ItemCoreClasses )
		{
			if ( Algo::Count( ItemCoreClasses, ItemCoreClass ) != Algo::Count( Other.ItemCoreClasses, ItemCoreClass ) )
			{
				return false;
			}
		}

		return true;
	}

};

FORCEINLINE uint32 GetTypeHash( const FFMGInvSysItemCoreClassArray& ArrayOfItemCoreClasses )
{
	return FCrc::MemCrc_DEPRECATED( &ArrayOfItemCoreClasses, sizeof( FFMGInvSysItemCoreClassArray ) );
}


USTRUCT()
struct FFMGInvSysCombineResult
{
	GENERATED_BODY()

public:

	FFMGInvSysCombineResult() { Successful = false; }

	FFMGInvSysCombineResult( bool InSuccessful, TArray<UFMGInvSysItemCore*> InResultItems )
	{
		Successful = InSuccessful;
		ResultItems = InResultItems;
	}

	bool Successful;

	TArray<UFMGInvSysItemCore*> ResultItems;

};

/**
 * 
 */
UCLASS( Blueprintable, BlueprintType )
class FMGINVSYS_API AFMGInvSysItemCombiner : public AActor
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	FFMGInvSysCombineResult CombineItems( TArray<UFMGInvSysItemCore*> SourceItems );

protected:

	/**
	 * A map of simple combinations like A + B + C --> D
	 */ 
	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	TMap<FFMGInvSysItemCoreClassArray, TSubclassOf<UFMGInvSysItemCore>> ClassMap;

	/**
	 * A map of combinations like A + B + C --> D + E + ...
	 */
	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	TMap<FFMGInvSysItemCoreClassArray, FFMGInvSysItemCoreClassArray> ClassMap_Multiple;

	/**
	 * A map that defines combination with functions.
	 * Those functions can perform custom logic during or after the combination.
	 */ 
	TMap<FFMGInvSysItemCoreClassArray, FFMGInvSysCombineDelegate> FunctionMap;

	//TMap<TSubclassOf<UItemCore>, FCombineFunction> FunctionMap_WildCards;
	
};
