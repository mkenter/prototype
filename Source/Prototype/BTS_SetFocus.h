// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_DefaultFocus.h"
#include "BTS_SetFocus.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UBTS_SetFocus final : public UBTService_DefaultFocus
{
	GENERATED_BODY()

	UBTS_SetFocus(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
