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

	FItemCoreClassArray GoldBarRecipe;
	GoldBarRecipe.ItemCoreClasses.Add( GoldCoinClass );
	GoldBarRecipe.ItemCoreClasses.Add( GoldCoinClass );
	ClassMap.Add( GoldBarRecipe, GoldBarClass );

	FItemCoreClassArray GoldBarAndMediumChinaUrn;
	GoldBarAndMediumChinaUrn.ItemCoreClasses.Add( GoldBarClass );
	GoldBarAndMediumChinaUrn.ItemCoreClasses.Add( MediumChinaUrnClass );
	FItemCoreClassArray FatGoldUrnAndPearlBracelet;
	FatGoldUrnAndPearlBracelet.ItemCoreClasses.Add( FatGoldUrnClass );
	FatGoldUrnAndPearlBracelet.ItemCoreClasses.Add( PearlBraceletClass );
	ClassMap_Multiple.Add( GoldBarAndMediumChinaUrn, FatGoldUrnAndPearlBracelet );
}

FItemCombinationResult AItemCombiner::CombineItems( TArray<UItemCore*> SourceItems, bool Directional )
{
	FItemCombinationResult Result;

// 	if ( Directional && SourceItems.Num() > 2 )
// 	{
// 		ensureAlwaysMsgf( false, TEXT( "Directional combine is to click an item, click a usage button, then click a secondary item." ) );
// 		return Result;
// 	}

	// TODO: FMGInvSys: Prevent combining with an item itself.

	// Gather UItemCore classes.
	FItemCoreClassArray SourceItemClassArray;
	for ( UItemCore* Item : SourceItems )
	{
		SourceItemClassArray.ItemCoreClasses.Add( Item->GetClass() );
	}

	// Search in ClassMap
	TSubclassOf<UItemCore>* pOutputClass = ClassMap.Find( SourceItemClassArray );
	if ( pOutputClass )
	{
		UItemCore* Output = NewObject<UItemCore>( this, *pOutputClass );

		Result.ResultItems.Add( Output );
		Result.Successful = true;

		return Result;
	}

	// Search in ClassMap_Multiple
	FItemCoreClassArray* pOutputClassArray = ClassMap_Multiple.Find( SourceItemClassArray );
	if ( pOutputClassArray )
	{
		for ( TSubclassOf<UItemCore> OutputClass : ( *pOutputClassArray ).ItemCoreClasses )
		{
			UItemCore* Output = NewObject<UItemCore>( this, OutputClass );

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

FItemCombinationResult AItemCombiner::MakeWhiteGem( TArray<UItemCore*> SourceItems )
{
	FItemCombinationResult Result;

	UE_LOG( LogTemp, Warning, TEXT( "Make white gem!" ) );

	return Result;
}
