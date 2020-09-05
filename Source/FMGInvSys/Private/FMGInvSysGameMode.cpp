// Copyright 2020 FreemanMakesGames https://www.freemanmakesgames.pro


#include "FMGInvSysGameMode.h"

#include "FMGInvSysItemCombiner.h"

const FVector ABSTRACT_SPAWN_POS = { 0, 0, 1000 };

void AFMGInvSysGameMode::StartPlay()
{
	ensureAlways( ItemCombinerClass );

	ItemCombiner = GetWorld()->SpawnActor<AFMGInvSysItemCombiner>( ItemCombinerClass, ABSTRACT_SPAWN_POS, FRotator::ZeroRotator );

	Super::StartPlay();
}
