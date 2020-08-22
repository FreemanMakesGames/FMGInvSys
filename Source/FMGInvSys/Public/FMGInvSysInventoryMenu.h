// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FMGInvSysItemUsage.h"

#include "CoreMinimal.h"
#include "FMGInvSysItemUsageButton.h"
#include "Blueprint/UserWidget.h"
#include "FMGInvSysInventoryMenu.generated.h"

class UFMGInvSysInventory;
class UFMGInvSysItemClicker;
class UFMGInvSysItemWidget;
class UFMGInvSysItemCore;
class IFMGInvSysInventoryOwner;

class UWrapBox;
class UVerticalBox;
class UButton;
class UTextBlock;

/**
 *
 */
UCLASS( Blueprintable )
class FMGINVSYS_API UFMGInvSysInventoryMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

protected:

	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	TSubclassOf<UFMGInvSysItemClicker> ItemClickerClass;

	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	TSubclassOf<UFMGInvSysItemUsageButton> ItemUsageButtonClass;

	UPROPERTY( meta = ( BindWidget ) )
	UWrapBox* WrapBox_Clickers;

	UPROPERTY( meta = ( BindWidget ) )
	UVerticalBox* ItemMenu;

	UPROPERTY( meta = ( BindWidget ) )
	UWrapBox* WrapBox_Clickers_Combining;

	UPROPERTY( meta = ( BindWidget ) )
	UButton* Button_AddToCombination;

	UPROPERTY( meta = ( BindWidget ) )
	UButton* Button_RemoveFromCombination;

	UPROPERTY( meta = ( BindWidget ) )
	UButton* Button_Combine;

	UPROPERTY( meta = ( BindWidget ) )
	UTextBlock* TextBlock_Description;

	UPROPERTY( meta = ( BindWidget ) )
	UButton* Button_Hide;

public:

	virtual void Setup( IFMGInvSysInventoryOwner* NewInventoryOwner );

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	virtual void Show();

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	virtual void Hide();

protected:

	virtual void SetupItemMenu();

	UFMGInvSysItemUsageButton* InitItemUsageButton( EFMGInvSysItemUsage ItemUsage );

	virtual UFMGInvSysItemClicker* AddNewItemClicker( UFMGInvSysItemCore* ItemCore );

	virtual void DisplayItemMenu( UFMGInvSysItemCore* ItemCore );

	virtual void RemoveItemClicker( UFMGInvSysItemCore* ItemCore );

	virtual void ResetLatestClicked();

protected:

	UFUNCTION()
	virtual void HandleOnInventoryUpdated( TArray<UFMGInvSysItemCore*> Added, TArray<UFMGInvSysItemCore*> Removed );

	UFUNCTION()
	virtual void HandleOnItemClickerClicked( UFMGInvSysItemClicker* Clicked );

	UFUNCTION()
	virtual void HandleOnItemUsageButtonClicked( UFMGInvSysItemUsageButton* ItemUsageButton );

	UFUNCTION()
	void HandleOnButtonAddToCombinationClicked();

	UFUNCTION()
	virtual void HandleOnButtonRemoveFromCombinationClicked();

	UFUNCTION()
	virtual void HandleOnButtonCombineClicked();

	UFUNCTION()
	virtual void HandleOnButtonHideClicked();

// Global variables for tracking
protected:

	IFMGInvSysInventoryOwner* InventoryOwner;

	UPROPERTY( BlueprintReadOnly, Category = "FMGInvSys" )
	TMap<UFMGInvSysItemCore*, UFMGInvSysItemClicker*> ItemToClicker;

	UPROPERTY()
	TMap<EFMGInvSysItemUsage, UFMGInvSysItemUsageButton*> AllItemUsagesToButtons;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere, Category = "FMGInvSys" )
	UFMGInvSysItemClicker* LatestClicked;

};
