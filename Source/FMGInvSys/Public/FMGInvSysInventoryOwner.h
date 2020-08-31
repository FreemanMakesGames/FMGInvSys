#pragma once

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"
#include "UObject/Interface.h"

#include "FMGInvSysInventoryOwner.generated.h"

class UFMGInvSysInventory;
class UFMGInvSysItemCore;

UINTERFACE( BlueprintType )
class FMGINVSYS_API UFMGInvSysInventoryOwner : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class FMGINVSYS_API IFMGInvSysInventoryOwner
{
	GENERATED_IINTERFACE_BODY()

public:

	virtual UFMGInvSysInventory* GetInventory() const PURE_VIRTUAL( , return nullptr; );

	virtual void ApplyItemUsage( UFMGInvSysItemCore* ItemCore, FString ItemUsage ) PURE_VIRTUAL( , );

	virtual void Server_CombineItems( const TArray<UFMGInvSysItemCore*>& SourceItemCores ) PURE_VIRTUAL( , );

};
