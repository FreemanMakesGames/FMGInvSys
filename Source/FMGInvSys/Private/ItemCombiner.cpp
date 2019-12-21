// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCombiner.h"

#include "ItemCore.h"

UItemCombiner::UItemCombiner()
{
	/* All possible combination among different classes of item cores. */


}

FItemCombinationResult UItemCombiner::CombineItems( TArray<UItemCore*> SourceItems, bool Directional )
{
	FItemCombinationResult Result;

// 	if ( Directional && SourceItems.Num() > 2 )
// 	{
// 		ensureAlwaysMsgf( false, TEXT( "Directional combine is to click an item, click a usage button, then click a secondary item." ) );
// 		return Result;
// 	}

	// TODO: Prevent combining with an item itself.

	// Gather UItemCore classes.
	FItemCoreClassSet SourceItemClasses;
	for ( int i = 0; i < SourceItems.Num(); i++ )
	{
		SourceItemClasses.ItemCoreClasses.Add( SourceItems[i]->GetClass() );
	}

	FCombineFunction* pCombineFunction = FunctionMap.Find( SourceItemClasses );

// 	if ( !pCombineFunction )
// 	{
// 		pCombineFunction = FunctionMap_WildCards.Find( SourceItemClassArrays)
// 	}

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
}
