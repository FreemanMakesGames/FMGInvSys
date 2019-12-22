// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCombiner.h"

#include "ItemCore.h"

void AItemCombiner::BeginPlay()
{
	Super::BeginPlay();

	/* All possible combination among different classes of item cores. */

	FItemCoreClassArray WhiteGemRecipe;
	WhiteGemRecipe.ItemCoreClasses.Add( RedGemClass );
	WhiteGemRecipe.ItemCoreClasses.Add( GreenGemClass );
	WhiteGemRecipe.ItemCoreClasses.Add( BlueGemClass );
	ClassMap.Add( WhiteGemRecipe, WhiteGemClass );
	FunctionMap.Add( WhiteGemRecipe, &AItemCombiner::MakeWhiteGem );
}

FItemCombinationResult AItemCombiner::CombineItems( TArray<UItemCore*> SourceItems, bool Directional )
{
	FItemCombinationResult Result;

// 	if ( Directional && SourceItems.Num() > 2 )
// 	{
// 		ensureAlwaysMsgf( false, TEXT( "Directional combine is to click an item, click a usage button, then click a secondary item." ) );
// 		return Result;
// 	}

	// TODO: Prevent combining with an item itself.

	// Gather UItemCore classes.
	FItemCoreClassArray SourceItemClassArray;
	for ( UItemCore* Item : SourceItems )
	{
		SourceItemClassArray.ItemCoreClasses.Add( Item->GetClass() );
	}

	FCombineFunction* pCombineFunction = FunctionMap.Find( SourceItemClassArray );

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

FItemCombinationResult AItemCombiner::MakeWhiteGem( TArray<UItemCore*> SourceItems )
{
	FItemCombinationResult Result;

	UE_LOG( LogTemp, Warning, TEXT( "Make white gem!" ) );

	return Result;
}
