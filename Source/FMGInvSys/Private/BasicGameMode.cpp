// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGameMode.h"

#include "ItemCombiner.h"

const FVector ABSTRACT_SPAWN_POS = { 0, 0, 1000 };

void ABasicGameMode::StartPlay()
{
	ensureAlways( ItemCombinerClass );

	ItemCombiner = GetWorld()->SpawnActor<AItemCombiner>( ItemCombinerClass, ABSTRACT_SPAWN_POS, FRotator::ZeroRotator );

	Super::StartPlay();
}

AItemCombiner* ABasicGameMode::GetItemCombiner()
{
	return ItemCombiner;
}
