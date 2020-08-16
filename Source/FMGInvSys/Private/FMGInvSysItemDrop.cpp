// Fill out your copyright notice in the Description page of Project Settings.


#include "FMGInvSysItemDrop.h"

#include "FMGInvSysItemCore.h"
#include "FMGInvSysItem.h"

// Sets default values for this component's properties
UFMGInvSysItemDrop::UFMGInvSysItemDrop()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFMGInvSysItemDrop::DropItem( UFMGInvSysItemCore* ItemCore )
{
	ItemCore->SpawnItem( GetComponentTransform() );
}
