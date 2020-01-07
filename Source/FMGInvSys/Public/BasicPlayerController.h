// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasicPlayerController.generated.h"

class UInventoryMenu;

/**
 * 
 */
UCLASS()
class FMGINVSYS_API ABasicPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

protected:

	virtual void OnPossess( APawn* PawnToPossess ) override;

protected:

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<UInventoryMenu> InventoryMenuClass;

	UPROPERTY()
	UInventoryMenu* InventoryMenu;

protected:

	void ToggleInventoryMenu();
	
};
