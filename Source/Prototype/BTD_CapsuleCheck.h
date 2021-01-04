// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomBTDecoratorBase.h"
#include "BTD_CapsuleCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UBTD_CapsuleCheck final : public UCustomBTDecoratorBase
{
	GENERATED_BODY()

	UBTD_CapsuleCheck();

	virtual bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const


override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float HeightOffset;

	
};
