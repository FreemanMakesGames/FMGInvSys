// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BasicCharacter.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Inventory.h"
#include "ItemDrop.h"
#include "ItemCore.h"
#include "Item.h"
#include "ItemCombiner.h"
#include "BasicGameMode.h"
#include "BasicPlayerController.h"

#include "Engine/EngineTypes.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// ABasicCharacter

ABasicCharacter::ABasicCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize( 42.f, 96.0f );

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator( 0.0f, 540.0f, 0.0f ); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>( TEXT( "CameraBoom" ) );
	CameraBoom->SetupAttachment( RootComponent );
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>( TEXT( "FollowCamera" ) );
	FollowCamera->SetupAttachment( CameraBoom, USpringArmComponent::SocketName ); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	Inventory = CreateDefaultSubobject<UInventory>( "Inventory" );

	ItemDrop = CreateDefaultSubobject<UItemDrop>( TEXT( "ItemDrop" ) );
	ItemDrop->SetupAttachment( RootComponent );
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABasicCharacter::SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent )
{
	// Set up gameplay key bindings
	check( PlayerInputComponent );
	PlayerInputComponent->BindAction( "Jump", IE_Pressed, this, &ACharacter::Jump );
	PlayerInputComponent->BindAction( "Jump", IE_Released, this, &ACharacter::StopJumping );

	PlayerInputComponent->BindAxis( "MoveForward", this, &ABasicCharacter::MoveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &ABasicCharacter::MoveRight );

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
	PlayerInputComponent->BindAxis( "TurnRate", this, &ABasicCharacter::TurnAtRate );
	PlayerInputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );
	PlayerInputComponent->BindAxis( "LookUpRate", this, &ABasicCharacter::LookUpAtRate );

	// handle touch devices
	PlayerInputComponent->BindTouch( IE_Pressed, this, &ABasicCharacter::TouchStarted );
	PlayerInputComponent->BindTouch( IE_Released, this, &ABasicCharacter::TouchStopped );

	// VR headset functionality
	PlayerInputComponent->BindAction( "ResetVR", IE_Pressed, this, &ABasicCharacter::OnResetVR );


	PlayerInputComponent->BindAction( "CollectItem", IE_Pressed, this, &ABasicCharacter::Server_CollectItem );
}

void ABasicCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ABasicCharacter::TouchStarted( ETouchIndex::Type FingerIndex, FVector Location )
{
	Jump();
}

void ABasicCharacter::TouchStopped( ETouchIndex::Type FingerIndex, FVector Location )
{
	StopJumping();
}

void ABasicCharacter::TurnAtRate( float Rate )
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput( Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() );
}

void ABasicCharacter::LookUpAtRate( float Rate )
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput( Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() );
}

void ABasicCharacter::MoveForward( float Value )
{
	if ( ( Controller != NULL ) && ( Value != 0.0f ) )
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		// get forward vector
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );
		AddMovementInput( Direction, Value );
	}
}

void ABasicCharacter::MoveRight( float Value )
{
	if ( ( Controller != NULL ) && ( Value != 0.0f ) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		// get right vector 
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );
		// add movement in that direction
		AddMovementInput( Direction, Value );
	}
}

void ABasicCharacter::ApplyItemUsage( UItemCore* ItemCore, EItemUsage ItemUsage )
{
	switch ( ItemUsage )
	{
	case EItemUsage::Dismantle:

		Dismantle( ItemCore );

		break;

	case EItemUsage::Equip:

		Equip( ItemCore );

		break;

	case EItemUsage::Drop:

		Server_Drop( ItemCore );

		break;

	case EItemUsage::Destroy:

		Destroy( ItemCore );

		break;

	default:

		ensureAlways( false );

	}
}

void ABasicCharacter::CombineItems( const TArray<UItemCore*>& SourceItemCores )
{
	///////////////////

	// TODO FMGInvSys:

	// As the author I'm not creating one AItemCombiner per inventory, simply because it's unnecessary.
	// I provided an ABasicGameMode, which holds an AItemCombiner,
	// So it can be easily found and referenced from other classes, like the sample code below.

	// But this may contradict with your game, or you may want to do it in other ways.
	// You likely need to write your own code here to find the AItemCombiner in the game.

	// Note that you can have more than one AItemCombiner. For example,
	// The game mode can give a normal AItemCombiner most of the time,
	// And give a "blessed" AItemCombiner if the player has acquired a skill,
	// Or has entered a special area.

	AItemCombiner* ItemCombiner = GetWorld()->GetAuthGameMode<ABasicGameMode>()->GetItemCombiner();

	if ( !ItemCombiner ) { ensureAlways( false ); return; }

	////////////////////////////////////////////////////////////////////////////////////////////////

	FItemCombinationResult Result = ItemCombiner->CombineItems( SourceItemCores, false );

	if ( !Result.Successful ) { return; }

	for ( UItemCore* ItemCore : SourceItemCores )
	{
		Destroy( ItemCore );
	}

	for ( UItemCore* ItemCore : Result.ResultItems )
	{
		Inventory->AddItem( ItemCore );
	}
}

// WTH if server collects item, then drops it, and Client then picks it UP, Client can actually drop it without crashing?
// And server can also picks it UP again too???
// But if Client picks UP, then drops, it crashes???
void ABasicCharacter::Server_CollectItem_Implementation()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> Filter;
	TArray<AActor*> Ignoring;
	TArray<AActor*> AdjacentActors;
	UKismetSystemLibrary::SphereOverlapActors( GetWorld(), GetActorLocation(), ItemCollectionRange, Filter, AItem::StaticClass(), Ignoring, AdjacentActors );

	for ( AActor* AdjacentActor : AdjacentActors )
	{
		UItemCore* ItemCore = Cast<AItem>( AdjacentActor )->GetItemCore();

		//ItemCore->Rename( nullptr, this );

		Inventory->AddItem( ItemCore );

		AdjacentActor->Destroy();
	}
}
bool ABasicCharacter::Server_CollectItem_Validate() { return true; }

void ABasicCharacter::Dismantle( UItemCore* ItemCore )
{
	for ( TPair<TSubclassOf<UItemCore>, int> DismantleResult : ItemCore->GetDismantleResults() )
	{
		for ( int i = 0; i < DismantleResult.Value; i++ )
		{
			UItemCore* Result = NewObject<UItemCore>( this, DismantleResult.Key );

			Inventory->AddItem( Result );
		}
	}

	if ( EquippedItem && EquippedItem->GetItemCore() == ItemCore )
	{
		EquippedItem->Destroy();
	}

	Inventory->RemoveItem( ItemCore );
}

// For now, only support to equip one item at a time.
// To support multiple item equipment, maybe make a new struct to hold socket name, and other info like attack and defense.
void ABasicCharacter::Equip( UItemCore* ItemCore )
{
	// Do nothing if the item is already equipped.
	if ( EquippedItem && EquippedItem->GetItemCore() == ItemCore ) { return; }

	if ( EquippedItem ) { EquippedItem->Destroy(); }

	// Spawn and attach new item.
	AItem* ItemToEquip = ItemCore->SpawnItem( FTransform::Identity );
	ItemToEquip->SetActorEnableCollision( false );
	ItemToEquip->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "RightHandSocket" );

	EquippedItem = ItemToEquip;
}

void ABasicCharacter::Server_Drop_Implementation( UItemCore* ItemCore )
{
	// If the item to drop is equipped,
	// Simply detach it and relocate it to item drop component,
	// And remove it from the inventory.
	if ( EquippedItem && EquippedItem->GetItemCore() == ItemCore )
	{
		EquippedItem->DetachAllSceneComponents( GetMesh(), FDetachmentTransformRules::KeepRelativeTransform );
		EquippedItem->SetActorLocation( ItemDrop->GetComponentLocation() );
		EquippedItem->SetActorEnableCollision( true ); // Re-enable collision, which was disabled when equipping this item.

		EquippedItem = nullptr;

		Inventory->RemoveItem( ItemCore );

		return;
	}

	// If the item to drop isn't equipped, spawn it at the item drop component.
	ItemCore->SpawnItem( ItemDrop->GetComponentTransform() );

	Inventory->RemoveItem( ItemCore );
}
bool ABasicCharacter::Server_Drop_Validate( UItemCore* ItemCore ) { return true; }

void ABasicCharacter::Destroy( UItemCore* ItemCore )
{
	if ( EquippedItem && EquippedItem->GetItemCore() == ItemCore )
	{
		EquippedItem->Destroy();
	}

	Inventory->RemoveItem( ItemCore );
}

// bool ABasicCharacter::ReplicateSubobjects( class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags )
// {
// 	bool bWroteSomething = Super::ReplicateSubobjects( Channel, Bunch, RepFlags );
// 
// 	bWroteSomething |= Channel->ReplicateSubobject( Inventory, *Bunch, *RepFlags );
// 
// 	for ( UItemCore* ItemCore : Inventory->GetItemCores() )
// 	{
// 		bWroteSomething |= Channel->ReplicateSubobject( ItemCore, *Bunch, *RepFlags );
// 	}
// 
//  	return bWroteSomething;
// }

void ABasicCharacter::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( ABasicCharacter, Inventory );
}
