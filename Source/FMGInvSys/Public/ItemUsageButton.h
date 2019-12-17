// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemUsage.h"

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "ItemUsageButton.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnClickedExt, UItemUsageButton*, ItemUsageButton );

/**
 * 
 */
UCLASS()
class FMGINVSYS_API UItemUsageButton : public UUserWidget
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

	EItemUsage GetItemUsage();

	void SetItemUsage( EItemUsage InItemUsage );

protected:

	EItemUsage ItemUsage;

protected:

	UFUNCTION()
	void HandleOnClicked();

};
