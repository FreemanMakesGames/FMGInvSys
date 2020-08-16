// Fill out your copyright notice in the Description page of Project Settings.


#include "FMGInvSysItemMenu.h"

#include "FMGInvSysItemUsageButton.h"
#include "FMGInvSysItemCore.h"

#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"

UFMGInvSysItemMenu::UFMGInvSysItemMenu( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{

}

void UFMGInvSysItemMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	EFMGInvSysItemUsage AllItemUsages[] = { EFMGInvSysItemUsage::Drop, EFMGInvSysItemUsage::Destroy };

	// Create one button for each item usage. They should always be reused and never be destroyed.
	for ( EFMGInvSysItemUsage ItemUsage : AllItemUsages )
	{
		UFMGInvSysItemUsageButton* ItemUsageButton = InitItemUsageButton( ItemUsage );

		ItemUsageButton->OnClickedExt.AddDynamic( this, &UFMGInvSysItemMenu::HandleOnItemUsageButtonClicked );

		AllItemUsagesToButtons.Add( ItemUsage, ItemUsageButton );
	}

	VerticalBox_Buttons->ClearChildren();
}

void UFMGInvSysItemMenu::Display( UFMGInvSysItemCore* ItemCore )
{
	CurrentItemCore = ItemCore;

	VerticalBox_Buttons->ClearChildren();

	for ( EFMGInvSysItemUsage ItemUsage : ItemCore->GetItemUsages() )
	{
		if ( UFMGInvSysItemUsageButton** pItemUsageButton = AllItemUsagesToButtons.Find( ItemUsage ) )
		{
			VerticalBox_Buttons->AddChildToVerticalBox( *pItemUsageButton );
		}
		else
		{
			UE_LOG( LogTemp, Error, TEXT( "UItemMenu found an EItemUsage from a UItemCore, that doesn't match any UItemUsageButton in the TMap." ) );
		}
	}
}

void UFMGInvSysItemMenu::HandleOnItemUsageButtonClicked( UFMGInvSysItemUsageButton* ItemUsageButton )
{
	OnItemUsageButtonClicked.Broadcast( ItemUsageButton->GetItemUsage(), CurrentItemCore );
}

UFMGInvSysItemUsageButton* UFMGInvSysItemMenu::InitItemUsageButton( EFMGInvSysItemUsage ItemUsage )
{
	UFMGInvSysItemUsageButton* ItemUsageButton = NewObject<UFMGInvSysItemUsageButton>( this );

	ItemUsageButton->SetItemUsage( ItemUsage );

	return ItemUsageButton;
}
