// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UItemWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Image_Icon->SetBrushSize( FVector2D( 150, 150 ) ); // TODO: FMGInvSys: Hardcoding.

	UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>( Image_Icon->Slot );
	ImageSlot->SetAutoSize( true );
}

void UItemWidget::SetItemCore( UItemCore* InItemCore )
{
	// TODO: ItemWidget: Find out if UItemWidget needs to know an UItemCore.
}
