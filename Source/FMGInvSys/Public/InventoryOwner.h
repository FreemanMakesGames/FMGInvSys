#pragma once

#include "InventoryOwner.generated.h"

class AItem;

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
	void Equip( AItem* Item );

};
