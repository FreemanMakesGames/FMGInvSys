// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemUsage.h"

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "ItemUsageButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnClickedExt, UItemUsageButton*, ItemUsageButton );

/**
 * 
 */
UCLASS()
class FMGINVSYS_API UItemUsageButton : public UButton
{
	GENERATED_BODY()

public:

	UItemUsageButton( const FObjectInitializer& ObjectInitializer );
	
public:

	FOnClickedExt OnClickedExt;

public:

	EItemUsage GetItemUsage();

	void SetItemUsage( EItemUsage InItemUsage );

protected:

	EItemUsage ItemUsage;

protected:

	UFUNCTION()
	void HandleOnClicked();

};
