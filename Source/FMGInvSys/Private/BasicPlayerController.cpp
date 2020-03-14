// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlayerController.h"

#include "InventoryMenu.h"
#include "InventoryOwner.h"

void ABasicPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Is this possible? Can player controller not have possessed any pawn after BeginPlay?
	if ( !InventoryMenu )
	{
		if ( InventoryMenuClass )
		{
			InventoryMenu = CreateWidget<UInventoryMenu>( this, InventoryMenuClass );
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

void ABasicPlayerController::OnPossess( APawn* PawnToPossess )
{
	Super::OnPossess( PawnToPossess );

	if ( !IsLocalController() )
		return;

	if ( IInventoryOwner* InventoryOwner = Cast<IInventoryOwner>( PawnToPossess ) )
	{
		// This is very possible because OnPossess happens before BeginPlay.
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
