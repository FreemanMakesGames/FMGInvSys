// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FMGInvSysGameMode.generated.h"

class AFMGInvSysItemCombiner;

/**
 * 
 */
UCLASS( Blueprintable )
class FMGINVSYS_API AFMGInvSysGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void StartPlay() override;
	
public:

	AFMGInvSysItemCombiner* GetItemCombiner() { return ItemCombiner; }

protected:

	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	TSubclassOf<AFMGInvSysItemCombiner> ItemCombinerClass;

	UPROPERTY()
	AFMGInvSysItemCombiner* ItemCombiner;

};
