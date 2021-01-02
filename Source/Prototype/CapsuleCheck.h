// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomBTDecoratorBase.h"
#include "CapsuleCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UCapsuleCheck final : public UCustomBTDecoratorBase
{
	GENERATED_BODY()

	UCapsuleCheck();

	virtual bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const


override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float HeightOffset;

	
};
