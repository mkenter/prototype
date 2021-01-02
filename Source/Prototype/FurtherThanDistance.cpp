// Fill out your copyright notice in the Description page of Project Settings.


#include "FurtherThanDistance.h"
#include "PrototypeCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UFurtherThanDistance::UFurtherThanDistance()
{
	NodeName = "Further Than Distance";

	bNotifyBecomeRelevant = true;
	bNotifyTick = true;

	bAllowAbortLowerPri = true;
	bAllowAbortNone = true;
	bAllowAbortChildNodes = true;
	FlowAbortMode = EBTFlowAbortMode::None;

	Distance = 600.f;
}

bool UFurtherThanDistance::CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// UE_LOG(LogTemp, Warning, TEXT("CalculateRawConditionValue"));
	
	const FName SelectedBlackboardKey = GetSelectedBlackboardKey();
	UObject* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelectedBlackboardKey);
	const APrototypeCharacter* Character = Cast<APrototypeCharacter>(Target);

	const UObject* SelfObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"));
	const AActor* SelfActor = Cast<AActor>(SelfObject);

	if (Character && SelfActor)
	{
		// UE_LOG(LogTemp, Warning, TEXT("UFurtherThanDistance: %f"), (Character->GetActorLocation() - SelfActor->GetActorLocation()).Size());
		// UE_LOG(LogTemp, Warning, TEXT("Regular"));
		return (Character->GetActorLocation() - SelfActor->GetActorLocation()).Size() >= Distance;
	}

	return false;
}
