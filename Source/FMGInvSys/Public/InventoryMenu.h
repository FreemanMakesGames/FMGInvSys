// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemUsage.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMenu.generated.h"

class UInventory;
class UItemClicker;
class UItemWidget;
class UItemCore;
class IInventoryOwner;

class UWrapBox;
class UVerticalBox;
class UButton;
class UTextBlock;

/**
 *
 */
UCLASS( Blueprintable )
class FMGINVSYS_API UInventoryMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

protected:

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<UItemClicker> ItemClickerClass;

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<UItemUsageButton> ItemUsageButtonClass;

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

	void Setup( IInventoryOwner* NewInventoryOwner );

	UFUNCTION( BlueprintCallable )
	void Show();

	UFUNCTION( BlueprintCallable )
	virtual void Hide();

protected:

	void SetupItemMenu();

	UItemUsageButton* InitItemUsageButton( EItemUsage ItemUsage );

	UItemClicker* AddNewItemClicker( UItemCore* ItemCore );

	void DisplayItemMenu( UItemCore* ItemCore );

	void RemoveItemClicker( UItemCore* ItemCore );

	void ResetLatestClicked();

public:

	UFUNCTION()
	virtual void HandleOnItemClickerClicked( UItemClicker* Clicked );

	UFUNCTION()
	void HandleOnItemUsageButtonClicked( UItemUsageButton* ItemUsageButton );

	UFUNCTION()
	void HandleOnButtonAddToCombinationClicked();

	UFUNCTION()
	void HandleOnButtonRemoveFromCombinationClicked();

	UFUNCTION()
	void HandleOnButtonCombineClicked();

	UFUNCTION()
	void HandleOnButtonHideClicked();

	UFUNCTION()
	void HandleOnItemAdded( UItemCore* ItemAdded );

	UFUNCTION()
	void HandleOnItemRemoved( UItemCore* ItemRemoved );

// Global variables for tracking
protected:

	IInventoryOwner* InventoryOwner;

	UPROPERTY( BlueprintReadOnly )
	TMap<UItemCore*, UItemClicker*> ItemToClicker;

	UPROPERTY()
	TMap<EItemUsage, UItemUsageButton*> AllItemUsagesToButtons;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	UItemClicker* LatestClicked;

};
