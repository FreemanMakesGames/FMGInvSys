// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemMenu.generated.h"

class UVerticalBox;
// class UItemUsageButton;
class UItemCore;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnItemUsageButtonClicked, UItemCore*, ItemCore );

/**
 * 
 */
UCLASS()
class FMGINVSYS_API UItemMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

public:

	UPROPERTY( BlueprintAssignable )
	FOnItemUsageButtonClicked OnItemUsageButtonClicked;

protected:

	UPROPERTY( meta = ( BindWidget ) )
	UVerticalBox* VerticalBox_Buttons;

	//ItemUsageButtons

	UPROPERTY( VisibleAnywhere )
	UItemCore* CurrentItemCore;

public:

	UFUNCTION()
	void Display( UItemCore* ItemCore );

protected:

// 	UFUNCTION()
// 	void HandleOnItemUsageButtonClicked();

};
