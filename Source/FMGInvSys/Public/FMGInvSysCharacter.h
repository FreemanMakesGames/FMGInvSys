// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FMGInvSysInventoryOwner.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FMGInvSysCharacter.generated.h"

class USceneComponent;
class UFMGInvSysInventory;
class AFMGInvSysItem;

UCLASS( config = Game )
class AFMGInvSysCharacter : public ACharacter, public IFMGInvSysInventoryOwner
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	class UCameraComponent* FollowCamera;

public:

	AFMGInvSysCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera )
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera )
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward( float Value );

	/** Called for side to side input */
	void MoveRight( float Value );

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate( float Rate );

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate( float Rate );

	/** Handler for when a touch input begins. */
	void TouchStarted( ETouchIndex::Type FingerIndex, FVector Location );

	/** Handler for when a touch input stops. */
	void TouchStopped( ETouchIndex::Type FingerIndex, FVector Location );

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	float ItemCollectionRange = 150;

public:

	virtual UFMGInvSysInventory* GetInventory() const override { return Inventory; }

	// Note: Although each method called by ApplyItemUsage is RPC, probably don't make itself RPC.
	//       What if there needs to be non-RPC code later?
	virtual void ApplyItemUsage( UFMGInvSysItemCore* ItemCore, FString ItemUsage ) override;

	UFUNCTION( Server, Reliable, WithValidation )
	virtual void Server_CombineItems( const TArray<UFMGInvSysItemCore*>& SourceItemCores ) override;
	virtual void Server_CombineItems_Implementation( const TArray<UFMGInvSysItemCore*>& SourceItemCores );
	virtual bool Server_CombineItems_Validate( const TArray<UFMGInvSysItemCore*>& SourceItemCores ) { return true; }

protected:

	UPROPERTY( Replicated, BlueprintReadOnly, Category = "FMGInvSys" )
	UFMGInvSysInventory* Inventory;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "FMGInvSys" )
	USceneComponent* ItemDropSpot;

protected:

	UFUNCTION( Server, Reliable, WithValidation )
	void Server_CollectItem();
	virtual void Server_CollectItem_Implementation();
	virtual bool Server_CollectItem_Validate() { return true; }

	UFUNCTION( Server, Reliable, WithValidation )
	void Server_Dismantle( UFMGInvSysItemCore* ItemCore );
	virtual void Server_Dismantle_Implementation( UFMGInvSysItemCore* ItemCore );
	virtual bool Server_Dismantle_Validate( UFMGInvSysItemCore* ItemCore ) { return true; }

	UFUNCTION( Server, Reliable, WithValidation )
	void Server_Equip( UFMGInvSysItemCore* ItemCore );
	virtual void Server_Equip_Implementation( UFMGInvSysItemCore* ItemCore );
	virtual bool Server_Equip_Validate( UFMGInvSysItemCore* ItemCore ) { return true; }

	UFUNCTION( Server, Reliable, WithValidation )
	void Server_Drop( UFMGInvSysItemCore* ItemCore, int Amount = 1 );
	virtual void Server_Drop_Implementation( UFMGInvSysItemCore* ItemCore, int Amount = 1 );
	virtual bool Server_Drop_Validate( UFMGInvSysItemCore* ItemCore, int Amount = 1 ) { return true; }

	UFUNCTION( Server, Reliable, WithValidation )
	void Server_Destroy( UFMGInvSysItemCore* ItemCore );
	virtual void Server_Destroy_Implementation( UFMGInvSysItemCore* ItemCore );
	virtual bool Server_Destroy_Validate( UFMGInvSysItemCore* ItemCore ) { return true; }

// public:
// 
// 	virtual bool ReplicateSubobjects( class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags ) override;

// Global tracking variables
protected:

	UPROPERTY()
	AFMGInvSysItem* EquippedItem;

};

