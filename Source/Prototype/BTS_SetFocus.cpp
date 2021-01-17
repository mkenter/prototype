// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_SetFocus.h"
#include "AIController.h"

UBTS_SetFocus::UBTS_SetFocus(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FocusPriority = EAIFocusPriority::Gameplay;
}
