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
UCLASS()
class FMGINVSYS_API UItemClicker : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

public:

	UPROPERTY( BlueprintAssignable )
	FOnButtonClicked OnButtonClicked;

public:

	UFUNCTION( BlueprintCallable )
	UItemCore* GetItemCore();

	UFUNCTION( BlueprintCallable )
	void SetItemCore( UItemCore* InItemCore );

protected:

	UPROPERTY( meta = ( BindWidget ), BlueprintReadOnly )
	UButton* Button;

	UPROPERTY( meta = ( BindWidget ) )
	UNamedSlot* ItemWidgetSlot;

	UPROPERTY()
	UItemCore* ItemCore;

protected:

	UFUNCTION()
	void HandleOnButtonClicked();

};