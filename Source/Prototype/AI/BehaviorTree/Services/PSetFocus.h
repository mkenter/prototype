// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_DefaultFocus.h"
#include "PSetFocus.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UPSetFocus final : public UBTService_DefaultFocus
{
	GENERATED_BODY()

	UPSetFocus(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
