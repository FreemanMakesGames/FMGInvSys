// Fill out your copyright notice in the Description page of Project Settings.


#include "FMGInvSysItem.h"

#include "Components/PrimitiveComponent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

#include "FMGInvSysItemCore.h"

AFMGInvSysItem::AFMGInvSysItem( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicatingMovement( true );
}

void AFMGInvSysItem::BeginPlay()
{
	ensureAlways( ItemCoreClass );
	
	// Item core may be needed in BP BeginPlay, so call this before Super::BeginPlay.
	// 
	// If this item is spawned from an existing item core,
	// Then the following new item core will be replaced in SetItemCore soon after.
	if ( HasAuthority() )
	{	
		ItemCore = NewObject<UFMGInvSysItemCore>( this, ItemCoreClass );

		if ( !ItemCore->IsStackable() )
			ensureAlwaysMsgf( InitialCount == 1, TEXT( "It's not supported to have an unstackable item with a more than 1 initial amount." ) );

		ItemCore->SetCount( InitialCount );
	}

	// This will execute BP BeginPlay.
	Super::BeginPlay();
}

void AFMGInvSysItem::SetItemCore( UFMGInvSysItemCore* InItemCore )
{
	ItemCore = InItemCore;
}

void AFMGInvSysItem::SetPhysicsEnabled_FromServer( bool Enabled )
{
	ensureAlways( GetNetMode() != ENetMode::NM_Client );

	SetPhysicsEnabled( Enabled );

	// This will trigger OnRep_IsPhysicsEnabled on clients.
	IsPhysicsEnabled = Enabled;
}

void AFMGInvSysItem::OnRep_IsPhysicsEnabled()
{
	SetPhysicsEnabled( IsPhysicsEnabled );
}

void AFMGInvSysItem::SetPhysicsEnabled( bool Enabled )
{
	SetActorEnableCollision( Enabled );

	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>( PrimitiveComponents );
	for ( UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents )
	{
		PrimitiveComponent->SetSimulatePhysics( Enabled );

		// Turning on physics simulation seems to lose parent,
		// So turning it off needs to re-attach to parent.
		if ( !Enabled )
			if ( PrimitiveComponent != GetRootComponent() )
				PrimitiveComponent->AttachToComponent( GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform );
	}
}

bool AFMGInvSysItem::ReplicateSubobjects( class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags )
{
	bool bWroteSomething = Super::ReplicateSubobjects( Channel, Bunch, RepFlags );

	bWroteSomething |= Channel->ReplicateSubobject( ItemCore, *Bunch, *RepFlags );

	return bWroteSomething;
}

void AFMGInvSysItem::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AFMGInvSysItem, ItemCore );
	DOREPLIFETIME( AFMGInvSysItem, IsPhysicsEnabled );
}
