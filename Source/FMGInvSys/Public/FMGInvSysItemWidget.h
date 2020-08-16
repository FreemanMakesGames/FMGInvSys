// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMGInvSysItemWidget.generated.h"

class UFMGInvSysItemCore;

class UImage;

/**
 * 
 */
UCLASS()
class FMGINVSYS_API UFMGInvSysItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	virtual void NativePreConstruct() override;

protected:

	// We are assuming that every item will have an icon representation in the inventory menu.
	// You can of course not assume it, and remove it from this base class,
	// And make a subclass like "ImageItemWidget" which has it.
	// Of course, if there's an item that doesn't have an icon representation,
	// You can simply hide this UImage too.
	UPROPERTY( meta = ( BindWidget ) )
	UImage* Image_Icon;

public:

	/** Initialize ItemWidget with ItemCore here. */
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "FMGInvSys" )
	void SetItemCore( UFMGInvSysItemCore* InItemCore );
	virtual void SetItemCore_Implementation( UFMGInvSysItemCore* InItemCore );

};
