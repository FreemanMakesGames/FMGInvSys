// Copyright 2020 FreemanMakesGames https://www.freemanmakesgames.pro


#include "FMGInvSysItemClicker.h"

#include "FMGInvSysItemCore.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"

void UFMGInvSysItemClicker::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Clicker->OnClicked.AddDynamic( this, &UFMGInvSysItemClicker::HandleOnButtonClicked );
}

void UFMGInvSysItemClicker::SetItemCore_Implementation( UFMGInvSysItemCore* InItemCore )
{
	ensureAlwaysMsgf( !ItemCore, TEXT( "Replacing an item core in an existing clicker?!" ) );
	
	ItemCore = InItemCore;

	ItemCore->OnDataChanged.AddDynamic( this, &UFMGInvSysItemClicker::HandleOnItemCoreDataChanged );

	UpdateCountText();

	// Give plugin user the convenience to have button image setup automatically.
	// But don't do it if they'll be using custom images for button's different states.
	if ( !bCustomButtonStyle )
	{
		UTexture2D* Icon = ItemCore->GetIcon();
		
		Clicker->WidgetStyle.Normal.SetResourceObject( Icon );
		Clicker->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;
		
		Clicker->WidgetStyle.Hovered.SetResourceObject( Icon );
		Clicker->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;
		
		Clicker->WidgetStyle.Pressed.SetResourceObject( Icon );
		Clicker->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
		
		Clicker->WidgetStyle.Disabled.SetResourceObject( Icon );
		Clicker->WidgetStyle.Disabled.DrawAs = ESlateBrushDrawType::Image;
	}
}

void UFMGInvSysItemClicker::UpdateCountText_Implementation()
{
	int OldCount = UKismetStringLibrary::Conv_StringToInt( Text_Count->GetText().ToString() );
	int NewCount = ItemCore->GetCount();
	
	Text_Count->SetText( FText::FromString( FString::FromInt( NewCount ) ) );

	if ( OldCount == 0 )
	{
		HighlightForNewClicker();
	}
	else
	{
		if ( NewCount > OldCount )
			HighlightForAddition();
		else if ( NewCount < OldCount )
			HighlightForSubtraction();
	}	
}
