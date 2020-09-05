// Copyright 2020 FreemanMakesGames https://www.freemanmakesgames.pro


#include "FMGInvSysItemUsageButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UFMGInvSysItemUsageButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_ItemUsage->OnClicked.AddDynamic( this, &UFMGInvSysItemUsageButton::HandleOnClicked );
}

void UFMGInvSysItemUsageButton::SetItemUsage( FString InItemUsage )
{
	ItemUsage = InItemUsage;
	
	TextBlock_ItemUsage->SetText( FText::FromString( ItemUsage ) );
}

void UFMGInvSysItemUsageButton::HandleOnClicked()
{
	OnClickedExt.Broadcast( this );
}
