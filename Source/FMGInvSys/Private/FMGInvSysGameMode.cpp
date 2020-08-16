// Fill out your copyright notice in the Description page of Project Settings.


#include "FMGInvSysGameMode.h"

#include "FMGInvSysItemCombiner.h"

const FVector ABSTRACT_SPAWN_POS = { 0, 0, 1000 };

void AFMGInvSysGameMode::StartPlay()
{
	ensureAlways( ItemCombinerClass );

	ItemCombiner = GetWorld()->SpawnActor<AFMGInvSysItemCombiner>( ItemCombinerClass, ABSTRACT_SPAWN_POS, FRotator::ZeroRotator );

	Super::StartPlay();
}

AFMGInvSysItemCombiner* AFMGInvSysGameMode::GetItemCombiner()
{
	return ItemCombiner;
}
