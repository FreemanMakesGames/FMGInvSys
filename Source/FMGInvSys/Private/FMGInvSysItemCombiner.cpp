// Copyright 2020 FreemanMakesGames https://www.freemanmakesgames.pro


#include "FMGInvSysItemCombiner.h"

#include "FMGInvSysItemCore.h"

void AFMGInvSysItemCombiner::BeginPlay()
{
	Super::BeginPlay();

	/* All possible combination among different classes of item cores. */

}

FFMGInvSysCombineResult AFMGInvSysItemCombiner::CombineItems( TArray<UFMGInvSysItemCore*> SourceItems )
{
	FFMGInvSysCombineResult Result;

	// TODO: FMGInvSys: Prevent combining with an item itself.

	// Gather UItemCore classes.
	FFMGInvSysItemCoreClassArray SourceItemClassArray;
	for ( UFMGInvSysItemCore* Item : SourceItems )
	{
		SourceItemClassArray.ItemCoreClasses.Add( Item->GetClass() );
	}

	// Search in ClassMap
	for ( auto Pair : ClassMap )
	{
		if ( Pair.Key == SourceItemClassArray )
		{
			UFMGInvSysItemCore* Output = NewObject<UFMGInvSysItemCore>( this, Pair.Value );

			Result.ResultItems.Add( Output );
			Result.Successful = true;
			return Result;
		}
	}

	// Search in ClassMap_Multiple
	for ( auto Pair : ClassMap_Multiple )
	{
		if ( Pair.Key == SourceItemClassArray )
		{
			for ( TSubclassOf<UFMGInvSysItemCore> OutputClass : Pair.Value.ItemCoreClasses )
			{
				UFMGInvSysItemCore* Output = NewObject<UFMGInvSysItemCore>( this, OutputClass );
				
				Result.ResultItems.Add( Output );
			}
			Result.Successful = true;
			return Result;
		}
	}

	// Search in FunctionMap
	for ( auto Pair : FunctionMap )
	{
		if ( Pair.Key == SourceItemClassArray )
		{
			return Pair.Value.Execute( SourceItems );
		}
	}

	// Empty unsuccessful result.
	return Result;

// 	if ( !pCombineFunction )
// 	{
// 		pCombineFunction = FunctionMap_WildCards.Find( SourceItemClassArrays)
// 	}
}
