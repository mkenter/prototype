// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayerController.h"
#include "Prototype/UI/PUserWidget.h"

void APPlayerController::CreateHUD()
{
	if (UIHUDWidget || !UIHUDWidgetClass)
	{
		return;
	}

	UIHUDWidget = CreateWidget<UPUserWidget>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();
}
