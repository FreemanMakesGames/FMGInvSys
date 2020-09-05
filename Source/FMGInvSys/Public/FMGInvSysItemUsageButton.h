// Copyright 2020 FreemanMakesGames https://www.freemanmakesgames.pro

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "FMGInvSysItemUsageButton.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnClickedExt, UFMGInvSysItemUsageButton*, ItemUsageButton );

/**
 * 
 */
UCLASS()
class FMGINVSYS_API UFMGInvSysItemUsageButton : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

public:

	template <typename WidgetT = UFMGInvSysItemUsageButton, typename OwnerT = UObject>
	static WidgetT* InitItemUsageButton( OwnerT* OwningObject, 
		TSubclassOf<UFMGInvSysItemUsageButton> ItemUsageButtonClass, FString ItemUsage )
	{
		WidgetT* ItemUsageButton = CreateWidget<WidgetT>( OwningObject, ItemUsageButtonClass );
		ItemUsageButton->SetItemUsage( ItemUsage );
		return ItemUsageButton;
	}
	
public:

	FOnClickedExt OnClickedExt;

protected:

	UPROPERTY( meta = ( BindWidget ) )
	UButton* Button_ItemUsage;

	UPROPERTY( meta = ( BindWidget ) )
	UTextBlock* TextBlock_ItemUsage;

	// In the BP sublcass, point it to the BP subclass itself..
	UPROPERTY( EditDefaultsOnly, Category = "FMGInvSys" )
	TSubclassOf<UFMGInvSysItemUsageButton> BPClass;

public:

	FString GetItemUsage() const { return ItemUsage; }

	virtual void SetItemUsage( FString InItemUsage );

protected:

	FString ItemUsage;

protected:

	UFUNCTION()
	virtual void HandleOnClicked();

};
