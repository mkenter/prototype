// Fill out your copyright notice in the Description page of Project Settings.


#include "PCanSeeTarget.h"
#include "Prototype/Characters/Enemies/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UPCanSeeTarget::UPCanSeeTarget()
{
	NodeName = "Can See Target";

	bNotifyBecomeRelevant = true;
	bNotifyTick = true;

	bAllowAbortLowerPri = true;
	bAllowAbortNone = true;
	bAllowAbortChildNodes = true;
	FlowAbortMode = EBTFlowAbortMode::None;
}

bool UPCanSeeTarget::CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const FName SelectedBlackboardKey = GetSelectedBlackboardKey();
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	
	const AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject((SelectedBlackboardKey)));

	const UObject* SelfObject = BlackboardComponent->GetValueAsObject(FName("SelfActor"));
	const AEnemy* SelfEnemy = Cast<AEnemy>(SelfObject);

	if (TargetActor && SelfEnemy)
	{
		FHitResult HitResult;
		const TArray<AActor*> ActorsToIgnore;
		
		return !UKismetSystemLibrary::LineTraceSingle(GetWorld(), SelfEnemy->GetActorLocation(), TargetActor->GetActorLocation(), TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FLinearColor::Green, FLinearColor::Red, 2.f);
	}

	return false;
}
