// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomBTDecoratorBase.h"
#include "BTD_FurtherThanDistance.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UBTD_FurtherThanDistance final : public UCustomBTDecoratorBase
{
	GENERATED_BODY()

	UBTD_FurtherThanDistance();

	virtual bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
		
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float Distance;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bUseActorValue;
	
};
