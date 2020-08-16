// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "FMGInvSysItemDrop.generated.h"

class UFMGInvSysItemCore;
class AFMGInvSysItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FMGINVSYS_API UFMGInvSysItemDrop : public USceneComponent
{
	GENERATED_BODY()

public:	

	UFMGInvSysItemDrop();

public:

	void DropItem( UFMGInvSysItemCore* ItemCore );

};
// TODO: Consider adding a visual mark.
