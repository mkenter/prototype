// Fill out your copyright notice in the Description page of Project Settings.


#include "PBTDecoratorBase.h"

bool UPBTDecoratorBase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return CalcConditionImpl(OwnerComp, NodeMemory);
}

void UPBTDecoratorBase::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FTNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<FTNodeInstanceMemory>(NodeMemory);
	
	const bool bResult = CalcConditionImpl(OwnerComp, NodeMemory);
	if (bResult != DecoratorMemory->bLastRawResult)
	{
		DecoratorMemory->bLastRawResult = bResult;
		OwnerComp.RequestExecution(this);
	}
}


bool UPBTDecoratorBase::CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return true;
}


void UPBTDecoratorBase::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FTNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<FTNodeInstanceMemory>(NodeMemory);
	DecoratorMemory->bLastRawResult = CalcConditionImpl(OwnerComp, NodeMemory);
}

uint16 UPBTDecoratorBase::GetInstanceMemorySize() const
{
	return sizeof(FTNodeInstanceMemory);
}
