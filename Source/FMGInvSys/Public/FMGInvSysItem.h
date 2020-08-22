// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMGInvSysItem.generated.h"

class UFMGInvSysItemCore;

UCLASS( Blueprintable, BlueprintType )
class FMGINVSYS_API AFMGInvSysItem : public AActor
{
	GENERATED_BODY()
	
public:	

	AFMGInvSysItem( const FObjectInitializer& ObjectInitializer );

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	TSubclassOf<UFMGInvSysItemCore> ItemCoreClass;

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	UFMGInvSysItemCore* GetItemCore() { return ItemCore; }

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void SetItemCore( UFMGInvSysItemCore* InItemCore );

protected:

	UPROPERTY( Replicated, BlueprintReadOnly, Category = "FMGInvSys" )
	UFMGInvSysItemCore* ItemCore;

public:

	/**
	 * Enable or disable collision in a replicated way.
	 * 
	 * This is mainly used for equipping and dropping items.
	 * Plugin user is free to write their own physics handling.
	 */
	void SetPhysicsEnabled_FromServer( bool Enabled );

protected:

	virtual void SetPhysicsEnabled( bool Enabled );

protected:

	UFUNCTION()
	void OnRep_IsPhysicsEnabled();

protected:

	UPROPERTY( ReplicatedUsing = OnRep_IsPhysicsEnabled )
	bool IsPhysicsEnabled = true;

public:

	virtual bool ReplicateSubobjects( class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags ) override;

};
