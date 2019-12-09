// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

class UItemCore;

/**
 * 
 */
UCLASS()
class FMGINVSYS_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/** Initialize ItemWidget with ItemCore here. */
	UFUNCTION( BlueprintCallable )
	virtual void SetItemCore( UItemCore* InItemCore ) PURE_VIRTUAL( , );

};
