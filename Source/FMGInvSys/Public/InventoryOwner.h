#pragma once

#include "InventoryOwner.generated.h"

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
	void Equip( UItemCore* ItemCore );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	void Drop( UItemCore* ItemCore );

};
