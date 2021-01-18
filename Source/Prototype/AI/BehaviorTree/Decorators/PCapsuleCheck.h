// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBTDecoratorBase.h"
#include "PCapsuleCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UPCapsuleCheck final : public UPBTDecoratorBase
{
	GENERATED_BODY()

	UPCapsuleCheck();

	virtual bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const


override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float HeightOffset;

	
};
