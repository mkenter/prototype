// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomBTDecoratorBase.h"

bool UCustomBTDecoratorBase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return CalcConditionImpl(OwnerComp, NodeMemory);
}

void UCustomBTDecoratorBase::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FTNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<FTNodeInstanceMemory>(NodeMemory);
	
	const bool bResult = CalcConditionImpl(OwnerComp, NodeMemory);
	if (bResult != DecoratorMemory->bLastRawResult)
	{
		DecoratorMemory->bLastRawResult = bResult;
		OwnerComp.RequestExecution(this);
	}
}


bool UCustomBTDecoratorBase::CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return true;
}


void UCustomBTDecoratorBase::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FTNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<FTNodeInstanceMemory>(NodeMemory);
	DecoratorMemory->bLastRawResult = CalcConditionImpl(OwnerComp, NodeMemory);
}

uint16 UCustomBTDecoratorBase::GetInstanceMemorySize() const
{
	return sizeof(FTNodeInstanceMemory);
}
