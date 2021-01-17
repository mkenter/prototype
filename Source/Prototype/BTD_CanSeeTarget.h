// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomBTDecoratorBase.h"
#include "BTD_CanSeeTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UBTD_CanSeeTarget final : public UCustomBTDecoratorBase
{
	GENERATED_BODY()

	UBTD_CanSeeTarget();

	virtual bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
