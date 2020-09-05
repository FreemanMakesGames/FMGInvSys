// Copyright 2020 FreemanMakesGames https://www.freemanmakesgames.pro

#pragma once

#include "FMGInvSysItemCore.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMGInvSysItemClicker.generated.h"

class UFMGInvSysItemCore;

class UButton;
class UTextBlock;

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
	UFMGInvSysItemCore* GetItemCore() const { return ItemCore; }

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

	UPROPERTY( meta = ( BindWidget ), BlueprintReadOnly, Category = "FMGInvSys" )
	UTextBlock* Text_Count;

protected:

	/**
	 * Set this to true if you want to do custom style for the button.
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "FMGInvSys" )
	bool bCustomButtonStyle = false;

public:

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "FMGInvSys" )
	void Redraw();
	virtual void Redraw_Implementation() { UpdateCountText(); }

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "FMGInvSys" )
	void UpdateCountText();
	virtual void UpdateCountText_Implementation();

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "FMGInvSys" )
	void HighlightForClicking();
	virtual void HighlightForClicking_Implementation() { }

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "FMGInvSys" )
	void HighlightForNewClicker();
	virtual void HighlightForNewClicker_Implementation() { }

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "FMGInvSys" )
	void HighlightForAddition();
	virtual void HighlightForAddition_Implementation() { }
	
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "FMGInvSys" )
	void HighlightForSubtraction();
	virtual void HighlightForSubtraction_Implementation() { }

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "FMGInvSys" )
	void Unhighlight();
	virtual void Unhighlight_Implementation() { }

protected:

	UPROPERTY( BlueprintReadWrite, Category = "FMGInvSys" )
	UFMGInvSysItemCore* ItemCore;

protected:

	UFUNCTION()
	virtual void HandleOnButtonClicked() { OnButtonClicked.Broadcast( this ); }

	UFUNCTION()
	virtual void HandleOnItemCoreDataChanged() { Redraw(); }

};
