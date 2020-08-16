// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FMGInvSysItemUsage.h"

#include "CoreMinimal.h"
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

	void Setup( IFMGInvSysInventoryOwner* NewInventoryOwner );

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void Show();

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	virtual void Hide();

protected:

	void SetupItemMenu();

	UFMGInvSysItemUsageButton* InitItemUsageButton( EFMGInvSysItemUsage ItemUsage );

	void Redraw();

	UFMGInvSysItemClicker* AddNewItemClicker( UFMGInvSysItemCore* ItemCore );

	void DisplayItemMenu( UFMGInvSysItemCore* ItemCore );

	void RemoveItemClicker( UFMGInvSysItemCore* ItemCore );

	void ResetLatestClicked();

protected:

	UFUNCTION()
	void HandleOnInventoryUpdated( TArray<UFMGInvSysItemCore*> Added, TArray<UFMGInvSysItemCore*> Removed );

	UFUNCTION()
	virtual void HandleOnItemClickerClicked( UFMGInvSysItemClicker* Clicked );

	UFUNCTION()
	void HandleOnItemUsageButtonClicked( UFMGInvSysItemUsageButton* ItemUsageButton );

	UFUNCTION()
	void HandleOnButtonAddToCombinationClicked();

	UFUNCTION()
	void HandleOnButtonRemoveFromCombinationClicked();

	UFUNCTION()
	void HandleOnButtonCombineClicked();

	UFUNCTION()
	void HandleOnButtonHideClicked();

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
