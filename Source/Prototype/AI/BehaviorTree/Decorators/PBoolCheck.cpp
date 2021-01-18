// Fill out your copyright notice in the Description page of Project Settings.


#include "PBoolCheck.h"

#include "BehaviorTree/BlackboardComponent.h"

UPBoolCheck::UPBoolCheck()
{
	NodeName = "Bool Check";

	bNotifyBecomeRelevant = true;
	bNotifyTick = true;

	bAllowAbortLowerPri = true;
	bAllowAbortNone = true;
	bAllowAbortChildNodes = true;
	FlowAbortMode = EBTFlowAbortMode::None;
}

bool UPBoolCheck::CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const FName SelectedBlackboardKey = GetSelectedBlackboardKey();
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	const bool BoolToCheck = BlackboardComponent->GetValueAsBool(SelectedBlackboardKey);
	
	return BoolToCheck;
}
