// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomBTDecoratorBase.h"
#include "FurtherThanDistance.generated.h"

struct FFurtherThanDistanceDecoratorMemory
{
	bool bLastRawResult;
};

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UFurtherThanDistance : public UCustomBTDecoratorBase
{
	GENERATED_BODY()

	UFurtherThanDistance();

	virtual bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
		
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float Distance;

};
