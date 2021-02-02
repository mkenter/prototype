// Fill out your copyright notice in the Description page of Project Settings.


#include "PFurtherThanDistance.h"
#include "Prototype/Characters/Enemies/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UPFurtherThanDistance::UPFurtherThanDistance()
{
	NodeName = "Further Than Distance";

	bNotifyBecomeRelevant = true;
	bNotifyTick = true;

	bAllowAbortLowerPri = true;
	bAllowAbortNone = true;
	bAllowAbortChildNodes = true;
	FlowAbortMode = EBTFlowAbortMode::None;

	Distance = 600.f;
	bUseActorValue = false;
}

bool UPFurtherThanDistance::CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// UE_LOG(LogTemp, Warning, TEXT("CalculateRawConditionValue"));
	
	const FName SelectedBlackboardKey = GetSelectedBlackboardKey();
	UObject* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelectedBlackboardKey);
	const AActor* TargetActor = Cast<AActor>(Target);

	const UObject* SelfObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"));
	const AEnemy* SelfEnemy = Cast<AEnemy>(SelfObject);

	float DistanceToUse = Distance;

	if (bUseActorValue && SelfEnemy)
	{
		DistanceToUse = SelfEnemy->KeepAtDistance;
	}

	if (TargetActor && SelfEnemy)
	{
		return (TargetActor->GetActorLocation() - SelfEnemy->GetActorLocation()).Size() >= DistanceToUse;
	}

	return false;
}
