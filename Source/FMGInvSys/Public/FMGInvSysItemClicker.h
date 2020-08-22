// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMGInvSysItemClicker.generated.h"

class UFMGInvSysItemCore;
class UFMGInvSysItemWidget;
class UButton;
class UNamedSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnButtonClicked, UFMGInvSysItemClicker*, Clicker );

/**
 * 
 */
UCLASS( Blueprintable )
class FMGINVSYS_API UFMGInvSysItemClicker : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

public:

	UPROPERTY( BlueprintAssignable )
	FOnButtonClicked OnButtonClicked;

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	UFMGInvSysItemCore* GetItemCore() { return ItemCore; }

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	void SetItemCore( UFMGInvSysItemCore* InItemCore );

protected:

	UPROPERTY( meta = ( BindWidget ), BlueprintReadOnly, Category = "FMGInvSys" )
	UButton* Clicker;

	UPROPERTY( meta = ( BindWidget ) )
	UNamedSlot* ItemWidgetSlot;

	UPROPERTY()
	UFMGInvSysItemCore* ItemCore;

public:

	UFUNCTION( BlueprintImplementableEvent )
	void HighlightForClicking();

	UFUNCTION( BlueprintImplementableEvent )
	void HighlightForAddition();

	UFUNCTION( BlueprintImplementableEvent )
	void Unhighlight();

protected:

	UFUNCTION()
	virtual void HandleOnButtonClicked();

};
