// Fill out your copyright notice in the Description page of Project Settings.


#include "FMGInvSysItemWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UFMGInvSysItemWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Image_Icon->SetBrushSize( FVector2D( 150, 150 ) ); // TODO: FMGInvSys: Hardcoding.

	UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>( Image_Icon->Slot );
	ImageSlot->SetAutoSize( true );
}

void UFMGInvSysItemWidget::SetItemCore_Implementation( UFMGInvSysItemCore* InItemCore )
{
	// Empty. This can be overriden in C++ subclasses, when needed.
}
