// Copyright 2020 FreemanMakesGames https://www.freemanmakesgames.pro

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FMGInvSysPlayerController.generated.h"

class UFMGInvSysInventoryMenu;

/**
 * 
 */
UCLASS()
class FMGINVSYS_API AFMGInvSysPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void OnPossess( APawn* PawnToPossess ) override;

protected:

	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	TSubclassOf<UFMGInvSysInventoryMenu> InventoryMenuClass;

	UPROPERTY()
	UFMGInvSysInventoryMenu* InventoryMenu;

protected:

	virtual void ToggleInventoryMenu();
	
};
