// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BasicGameMode.generated.h"

class AItemCombiner;

/**
 * 
 */
UCLASS( Blueprintable )
class FMGINVSYS_API ABasicGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void StartPlay() override;
	
public:

	AItemCombiner* GetItemCombiner();

protected:

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<AItemCombiner> ItemCombinerClass;

	AItemCombiner* ItemCombiner;

};
