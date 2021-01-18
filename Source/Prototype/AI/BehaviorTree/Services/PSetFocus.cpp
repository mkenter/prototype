// Fill out your copyright notice in the Description page of Project Settings.


#include "PSetFocus.h"
#include "AIController.h"

UPSetFocus::UPSetFocus(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FocusPriority = EAIFocusPriority::Gameplay;
}
