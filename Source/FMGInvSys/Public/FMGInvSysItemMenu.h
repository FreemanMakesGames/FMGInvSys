// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FMGInvSysItemUsage.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMGInvSysItemMenu.generated.h"

class UVerticalBox;
class UFMGInvSysItemUsageButton;
class UFMGInvSysItemCore;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnItemUsageButtonClicked, EFMGInvSysItemUsage, ItemUsage, UFMGInvSysItemCore*, ItemCore );

/**
 * 
 */
UCLASS()
class FMGINVSYS_API UFMGInvSysItemMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UFMGInvSysItemMenu( const FObjectInitializer& ObjectInitializer );

protected:

	virtual void NativeOnInitialized() override;

public:

	UPROPERTY( BlueprintAssignable )
	FOnItemUsageButtonClicked OnItemUsageButtonClicked;

protected:

	UPROPERTY( meta = ( BindWidget ) )
	UVerticalBox* VerticalBox_Buttons;

	TMap<EFMGInvSysItemUsage, UFMGInvSysItemUsageButton*> AllItemUsagesToButtons;

	UPROPERTY( VisibleAnywhere, Category = "FMGInvSys" )
	UFMGInvSysItemCore* CurrentItemCore;

public:

	UFUNCTION()
	void Display( UFMGInvSysItemCore* ItemCore );

protected:

	UFUNCTION()
	void HandleOnItemUsageButtonClicked( UFMGInvSysItemUsageButton* ItemUsageButton );

protected:

	UFMGInvSysItemUsageButton* InitItemUsageButton( EFMGInvSysItemUsage ItemUsage );

};
