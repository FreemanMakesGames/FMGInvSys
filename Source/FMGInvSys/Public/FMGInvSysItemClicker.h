// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FMGInvSysItemCore.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMGInvSysItemClicker.generated.h"

class UFMGInvSysItemCore;
class UButton;

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

	template <typename WidgetT = UFMGInvSysItemClicker, typename OwnerT = UObject>
	static WidgetT* InitItemClicker( OwnerT* OwningObject,
		TSubclassOf<UFMGInvSysItemClicker> ItemClickerClass, UFMGInvSysItemCore* ItemCore )
	{
		WidgetT* ItemClicker = CreateWidget<WidgetT>( OwningObject, ItemClickerClass );
		ItemClicker->SetItemCore( ItemCore );
		return ItemClicker;
	}

public:

	UPROPERTY( BlueprintAssignable )
	FOnButtonClicked OnButtonClicked;

public:

	UFUNCTION( BlueprintCallable, Category = "FMGInvSys" )
	UFMGInvSysItemCore* GetItemCore() { return ItemCore; }

	/**
	 * Override this method, either in C++ or BP,
	 * To also set up custom appearance based on the item core,
	 */
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "FMGInvSys" )
	void SetItemCore( UFMGInvSysItemCore* InItemCore );
	virtual void SetItemCore_Implementation( UFMGInvSysItemCore* InItemCore );

protected:

	UPROPERTY( meta = ( BindWidget ), BlueprintReadOnly, Category = "FMGInvSys" )
	UButton* Clicker;

protected:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "FMGInvSys" )
	bool bUseCustomIcons;

public:

	UFUNCTION( BlueprintImplementableEvent )
	void HighlightForClicking();

	UFUNCTION( BlueprintImplementableEvent )
	void HighlightForAddition();

	UFUNCTION( BlueprintImplementableEvent )
	void Unhighlight();

protected:

	UPROPERTY()
	UFMGInvSysItemCore* ItemCore;

protected:

	UFUNCTION()
	virtual void HandleOnButtonClicked();

};
