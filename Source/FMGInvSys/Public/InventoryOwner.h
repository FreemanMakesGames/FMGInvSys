#pragma once

#include "ItemUsage.h"

#include "InventoryOwner.generated.h"

class UInventory;
class UItemCore;

UINTERFACE( BlueprintType )
class FMGINVSYS_API UInventoryOwner : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class FMGINVSYS_API IInventoryOwner
{
	GENERATED_IINTERFACE_BODY()

public:

	virtual UInventory* GetInventory() PURE_VIRTUAL( , return nullptr; );

	virtual void ApplyItemUsage( UItemCore* ItemCore, EItemUsage ItemUsage ) PURE_VIRTUAL( , );

	virtual void CombineItems( const TArray<UItemCore*>& SourceItemCores ) PURE_VIRTUAL( , );

};
