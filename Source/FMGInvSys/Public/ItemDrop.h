// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ItemDrop.generated.h"

class UItemCore;
class AItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FMGINVSYS_API UItemDrop : public USceneComponent
{
	GENERATED_BODY()

public:	

	UItemDrop();

public:

	void DropItem( UItemCore* ItemCore );

};
// TODO: Consider adding a visual mark.
