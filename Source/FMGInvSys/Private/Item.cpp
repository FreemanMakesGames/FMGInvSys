// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/PrimitiveComponent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

#include "ItemCore.h"
#include "ItemWidget.h"

AItem::AItem( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicatingMovement( true );
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	ensureAlways( ItemCoreClass );

	// This is for cases like when a level designer placed an AItem into the level manually.
	if ( !ItemCore )
	{
		ItemCore = NewObject<UItemCore>( this, ItemCoreClass );
	}
}

UItemCore* AItem::GetItemCore()
{
	return ItemCore;
}

void AItem::SetItemCore( UItemCore* InItemCore )
{
// 	if ( !ItemCore )
// 	{
// 		ItemCore = InItemCore;
// 	}
// 	else
// 	{
// 		ensureAlways( false );
// 		return;
// 	}

	ItemCore = InItemCore;
}

void AItem::SetPhysicsEnabled_FromServer( bool Enabled )
{
	ensureAlways( GetNetMode() != ENetMode::NM_Client );

	SetPhysicsEnabled( Enabled );

	// This will trigger OnRep_IsPhysicsEnabled on clients.
	IsPhysicsEnabled = Enabled;
}

void AItem::OnRep_IsPhysicsEnabled()
{
	SetPhysicsEnabled( IsPhysicsEnabled );
}

void AItem::SetPhysicsEnabled( bool Enabled )
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

bool AItem::ReplicateSubobjects( class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags )
{
	bool bWroteSomething = Super::ReplicateSubobjects( Channel, Bunch, RepFlags );

	bWroteSomething |= Channel->ReplicateSubobject( ItemCore, *Bunch, *RepFlags );

	return bWroteSomething;
}

void AItem::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AItem, ItemCore );
	DOREPLIFETIME( AItem, IsPhysicsEnabled );
}
