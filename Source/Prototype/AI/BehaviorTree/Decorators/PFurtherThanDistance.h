// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBTDecoratorBase.h"
#include "PFurtherThanDistance.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UPFurtherThanDistance final : public UPBTDecoratorBase
{
	GENERATED_BODY()

	UPFurtherThanDistance();

	virtual bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
		
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float Distance;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bUseActorValue;
	
};
