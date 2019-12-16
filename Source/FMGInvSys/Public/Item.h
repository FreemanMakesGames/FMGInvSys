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

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<UItemCore> ItemCoreClass;

public:

	UFUNCTION( BlueprintCallable )
	UItemCore* GetItemCore();

	UFUNCTION( BlueprintCallable )
	void SetItemCore( UItemCore* InItemCore );

protected:

	UPROPERTY( BlueprintReadOnly )
	UItemCore* ItemCore;

public:	

	// Replicate Subobject?

};
