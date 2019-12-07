// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemUsage.generated.h"

UENUM( BlueprintType )
enum class EItemUsage : uint8
{
	Destroy		UMETA( DisplayName = "Destroy" ),
	Drop		UMETA( DisplayName = "Drop" )
};
