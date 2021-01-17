// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayerController.h"
#include "PUserWidget.h"

void APPlayerController::CreateHUD()
{
	if (UIHUDWidget)
	{
		return;
	}

	if (!UIHUDWidgetClass)
	{
		return;
	}

	UIHUDWidget = CreateWidget<UPUserWidget>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();
}
