// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlayerController.h"

#include "InventoryMenu.h"
#include "InventoryOwner.h"

/**
 * On server, this happens before OnPossess, for remote player controllers.
 */
void ABasicPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if ( !IsLocalController() )
		return;

	if ( !InventoryMenu )
	{
		// It looks like the code here will only run on clients,
		// Because listen server would have created InventoryMenu in OnPossess already.

		if ( InventoryMenuClass )
		{
			InventoryMenu = CreateWidget<UInventoryMenu>( this, InventoryMenuClass );

			InventoryMenu->Setup( Cast<IInventoryOwner>( GetPawn() ) );
		}
		else { ensureAlways( false ); }
	}
}

void ABasicPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if ( IsLocalController() )
		InputComponent->BindAction( "OpenInventory", IE_Pressed, this, &ABasicPlayerController::ToggleInventoryMenu );
}

/**
 * This isn't fired on clients.
 */
void ABasicPlayerController::OnPossess( APawn* PawnToPossess )
{
	Super::OnPossess( PawnToPossess );

	if ( !IsLocalController() )
		return;

	if ( IInventoryOwner* InventoryOwner = Cast<IInventoryOwner>( PawnToPossess ) )
	{
		// On server, OnPossess happens before BeginPlay.
		if ( !InventoryMenu )
		{
			if ( InventoryMenuClass )
			{
				InventoryMenu = CreateWidget<UInventoryMenu>( this, InventoryMenuClass );

				InventoryMenu->Setup( InventoryOwner );
			}
			else { ensureAlways( false ); return; }
		}
	}
	else
	{
		// Disable whichever input that opens the inventory menu,
		// Because the newly possessed pawn isn't an IInventoryOwner.
	}
}

void ABasicPlayerController::ToggleInventoryMenu()
{
	if ( InventoryMenu->IsInViewport() )
	{
		InventoryMenu->Hide();

		SetInputMode( FInputModeGameOnly() );

		bShowMouseCursor = false;
	}
	else
	{
		InventoryMenu->Show();

		SetInputMode( FInputModeGameAndUI() );

		bShowMouseCursor = true;
	}
}
