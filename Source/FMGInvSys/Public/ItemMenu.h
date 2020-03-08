// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemUsage.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemMenu.generated.h"

class UVerticalBox;
class UItemUsageButton;
class UItemCore;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnItemUsageButtonClicked, EItemUsage, ItemUsage, UItemCore*, ItemCore );

/**
 * 
 */
UCLASS()
class FMGINVSYS_API UItemMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UItemMenu( const FObjectInitializer& ObjectInitializer );

protected:

	virtual void NativeOnInitialized() override;

public:

	UPROPERTY( BlueprintAssignable )
	FOnItemUsageButtonClicked OnItemUsageButtonClicked;

protected:

	UPROPERTY( meta = ( BindWidget ) )
	UVerticalBox* VerticalBox_Buttons;

	TMap<EItemUsage, UItemUsageButton*> AllItemUsagesToButtons;

	UPROPERTY( VisibleAnywhere, Category = "FMGInvSys" )
	UItemCore* CurrentItemCore;

public:

	UFUNCTION()
	void Display( UItemCore* ItemCore );

protected:

	UFUNCTION()
	void HandleOnItemUsageButtonClicked( UItemUsageButton* ItemUsageButton );

protected:

	UItemUsageButton* InitItemUsageButton( EItemUsage ItemUsage );

};
