// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBTDecoratorBase.h"
#include "PBoolCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UPBoolCheck final : public UPBTDecoratorBase
{
	GENERATED_BODY()
	
	UPBoolCheck();

	virtual bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
