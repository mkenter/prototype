// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBTDecoratorBase.h"
#include "PCanSeeTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UPCanSeeTarget final : public UPBTDecoratorBase
{
	GENERATED_BODY()

	UPCanSeeTarget();

	virtual bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
