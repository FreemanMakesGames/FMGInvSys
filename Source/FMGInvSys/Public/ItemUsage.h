// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemUsage.generated.h"

UENUM( BlueprintType )
enum class EItemUsage : uint8
{
	Use			UMETA( DisplayName = "Use" ),

	Eat			UMETA( DisplayName = "Eat" ),
	Drink		UMETA( DisplayName = "Drink" ),
	Heal		UMETA( DisplayName = "Heal" ),

	Equip		UMETA( DisplayName = "Equip" ),

	Dismantle	UMETA( DisplayName = "Dismantle" ),
	Drop		UMETA( DisplayName = "Drop" ),
	Destroy		UMETA( DisplayName = "Destroy" )
};
