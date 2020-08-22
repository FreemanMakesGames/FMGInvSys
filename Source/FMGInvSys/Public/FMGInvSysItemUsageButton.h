// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "FMGInvSysItemUsageButton.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnClickedExt, UFMGInvSysItemUsageButton*, ItemUsageButton );

/**
 * 
 */
UCLASS()
class FMGINVSYS_API UFMGInvSysItemUsageButton : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;
	
public:

	FOnClickedExt OnClickedExt;

protected:

	UPROPERTY( meta = ( BindWidget ) )
	UButton* Button_ItemUsage;

	UPROPERTY( meta = ( BindWidget ) )
	UTextBlock* TextBlock_ItemUsage;

public:

	FString GetItemUsage() { return ItemUsage; }

	virtual void SetItemUsage( FString InItemUsage );

protected:

	FString ItemUsage;

protected:

	UFUNCTION()
	virtual void HandleOnClicked();

};
