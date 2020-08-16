// Fill out your copyright notice in the Description page of Project Settings.


#include "FMGInvSysItemCombiner.h"

#include "FMGInvSysItemCore.h"

void AFMGInvSysItemCombiner::BeginPlay()
{
	Super::BeginPlay();

	/* All possible combination among different classes of item cores. */

}

FFMGInvSysItemCombinationResult AFMGInvSysItemCombiner::CombineItems( TArray<UFMGInvSysItemCore*> SourceItems, bool Directional )
{
	FFMGInvSysItemCombinationResult Result;

// 	if ( Directional && SourceItems.Num() > 2 )
// 	{
// 		ensureAlwaysMsgf( false, TEXT( "Directional combine is to click an item, click a usage button, then click a secondary item." ) );
// 		return Result;
// 	}

	// TODO: FMGInvSys: Prevent combining with an item itself.

	// Gather UItemCore classes.
	FFMGInvSysItemCoreClassArray SourceItemClassArray;
	for ( UFMGInvSysItemCore* Item : SourceItems )
	{
		SourceItemClassArray.ItemCoreClasses.Add( Item->GetClass() );
	}

	// Search in ClassMap
	TSubclassOf<UFMGInvSysItemCore>* pOutputClass = ClassMap.Find( SourceItemClassArray );
	if ( pOutputClass )
	{
		UFMGInvSysItemCore* Output = NewObject<UFMGInvSysItemCore>( this, *pOutputClass );

		Result.ResultItems.Add( Output );
		Result.Successful = true;

		return Result;
	}

	// Search in ClassMap_Multiple
	FFMGInvSysItemCoreClassArray* pOutputClassArray = ClassMap_Multiple.Find( SourceItemClassArray );
	if ( pOutputClassArray )
	{
		for ( TSubclassOf<UFMGInvSysItemCore> OutputClass : ( *pOutputClassArray ).ItemCoreClasses )
		{
			UFMGInvSysItemCore* Output = NewObject<UFMGInvSysItemCore>( this, OutputClass );

			Result.ResultItems.Add( Output );
		}

		Result.Successful = true;

		return Result;
	}

	// Search in FunctionMap
	FCombineFunction* pCombineFunction = FunctionMap.Find( SourceItemClassArray );
	if ( pCombineFunction )
	{
		FCombineFunction CombineFunction = *pCombineFunction;

		Result = ( this->*( CombineFunction ) )( SourceItems );

		return Result;
	}
	else
	{
		return Result; // Empty unsuccessful result.
	}

// 	if ( !pCombineFunction )
// 	{
// 		pCombineFunction = FunctionMap_WildCards.Find( SourceItemClassArrays)
// 	}
}

FFMGInvSysItemCombinationResult AFMGInvSysItemCombiner::MakeWhiteGem( TArray<UFMGInvSysItemCore*> SourceItems )
{
	FFMGInvSysItemCombinationResult Result;

	UE_LOG( LogTemp, Warning, TEXT( "Make white gem!" ) );

	return Result;
}
