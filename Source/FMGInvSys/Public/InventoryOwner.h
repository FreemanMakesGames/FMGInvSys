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

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	UInventory* GetInventory();

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	void ApplyItemUsage( UItemCore* ItemCore, EItemUsage ItemUsage );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	void CombineItems( const TArray<UItemCore*>& SourceItems );

};
