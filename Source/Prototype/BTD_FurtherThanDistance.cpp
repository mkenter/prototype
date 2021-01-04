// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_FurtherThanDistance.h"

#include "Enemy.h"
#include "PrototypeCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_FurtherThanDistance::UBTD_FurtherThanDistance()
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

bool UBTD_FurtherThanDistance::CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// UE_LOG(LogTemp, Warning, TEXT("CalculateRawConditionValue"));
	
	const FName SelectedBlackboardKey = GetSelectedBlackboardKey();
	UObject* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelectedBlackboardKey);
	const APrototypeCharacter* Character = Cast<APrototypeCharacter>(Target);

	const UObject* SelfObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"));
	const AEnemy* SelfEnemy = Cast<AEnemy>(SelfObject);

	float DistanceToUse = Distance;

	if (bUseActorValue && SelfEnemy)
	{
		DistanceToUse = SelfEnemy->KeepAtDistance;
	}

	if (Character && SelfEnemy)
	{
		// UE_LOG(LogTemp, Warning, TEXT("UFurtherThanDistance: %f"), (Character->GetActorLocation() - SelfActor->GetActorLocation()).Size());
		// UE_LOG(LogTemp, Warning, TEXT("Regular"));
		return (Character->GetActorLocation() - SelfEnemy->GetActorLocation()).Size() >= DistanceToUse;
	}

	return false;
}
