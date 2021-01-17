// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "CustomBTDecoratorBase.generated.h"

struct FDecoratorMemory
{
	bool bLastRawResult;
};

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UCustomBTDecoratorBase : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
	typedef FDecoratorMemory FTNodeInstanceMemory;

	virtual uint16 GetInstanceMemorySize() const override;

	virtual bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	
protected:

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


};
