// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomBTDecoratorBase.h"
#include "BTD_BoolCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UBTD_BoolCheck final : public UCustomBTDecoratorBase
{
	GENERATED_BODY()
	
	UBTD_BoolCheck();

	virtual bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
