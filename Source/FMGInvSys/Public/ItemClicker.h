// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemClicker.generated.h"

class UItemCore;
class UItemWidget;
class UButton;
class UNamedSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnButtonClicked, UItemClicker*, Clicker );

/**
 * 
 */
UCLASS( Blueprintable )
class FMGINVSYS_API UItemClicker : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

public:

	UPROPERTY( BlueprintAssignable )
	FOnButtonClicked OnButtonClicked;

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	UItemCore* GetItemCore();

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void SetItemCore( UItemCore* InItemCore );

protected:

	UPROPERTY( meta = ( BindWidget ), BlueprintReadOnly, Category = "FMGInvSys" )
	UButton* Clicker;

	UPROPERTY( meta = ( BindWidget ) )
	UNamedSlot* ItemWidgetSlot;

	UPROPERTY()
	UItemCore* ItemCore;

public:

	UFUNCTION( BlueprintImplementableEvent )
	void HighlightForClicking();

	UFUNCTION( BlueprintImplementableEvent )
	void HighlightForAddition();

	UFUNCTION( BlueprintImplementableEvent )
	void Unhighlight();

protected:

	UFUNCTION()
	void HandleOnButtonClicked();

};
