// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMenu.generated.h"

class UInventory;
class UItemClicker;
class UItemWidget;
class UItemCore;

class UWrapBox;
class UButton;
class UItemMenu;

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

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TSubclassOf<UItemClicker> ItemClickerClass;

	UPROPERTY( meta = ( BindWidget ) )
	UItemMenu* ItemMenu;

	UPROPERTY( meta = ( BindWidget ) )
	UWrapBox* WrapBox_ItemClickers;

	UPROPERTY( meta = ( BindWidget ) )
	UButton* Button_Hide;

	UPROPERTY()
	UInventory* Inventory;

public:

	UFUNCTION( BlueprintCallable )
	void Setup( UInventory* InInventory );

	UFUNCTION( BlueprintCallable )
	void Show();

	UFUNCTION( BlueprintCallable )
	virtual void Hide();

protected:

	UItemClicker* AddNewItemClicker( UItemCore* ItemCore );

public:

	UFUNCTION()
	virtual void HandleOnItemClickerClicked( UItemClicker* Clicked );

	UFUNCTION()
	void HandleOnButtonHideClicked();

	UFUNCTION()
	void HandleOnItemAdded( UItemCore* ItemAdded );

	UFUNCTION()
	void HandleOnItemRemoved( UItemCore* ItemRemoved );

protected:

	UPROPERTY( BlueprintReadOnly )
	TMap<UItemCore*, UItemClicker*> ItemToClicker;

	UPROPERTY( VisibleAnywhere )
	UItemCore* FirstItemForCombination;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	bool IsCombining;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	UItemClicker* LastClicked;

};
