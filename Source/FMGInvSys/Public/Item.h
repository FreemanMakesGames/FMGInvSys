// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemUsage.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UItemCore;

UCLASS( Blueprintable, BlueprintType )
class FMGINVSYS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	

	AItem( const FObjectInitializer& ObjectInitializer );

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	TSubclassOf<UItemCore> ItemCoreClass;

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	UItemCore* GetItemCore();

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void SetItemCore( UItemCore* InItemCore );

protected:

	UPROPERTY( Replicated, BlueprintReadOnly, Category = "FMGInvSys" )
	UItemCore* ItemCore;

public:

	/**
	 * Enable or disable collision in a replicated way.
	 */
	void SetPhysicsEnabled_FromServer( bool Enabled );

protected:

	void SetPhysicsEnabled( bool Enabled );

protected:

	UFUNCTION()
	void OnRep_IsPhysicsEnabled();

protected:

	UPROPERTY( ReplicatedUsing = OnRep_IsPhysicsEnabled )
	bool IsPhysicsEnabled = true;

public:

	virtual bool ReplicateSubobjects( class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags ) override;

};
