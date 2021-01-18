// Fill out your copyright notice in the Description page of Project Settings.


#include "PCanSeeTarget.h"


#include "../../../Characters/Enemies/Enemy.h"
#include "../../../Characters/Player/PrototypeCharacter.h"
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
	
	const APrototypeCharacter* Character = Cast<APrototypeCharacter>(BlackboardComponent->GetValueAsObject((SelectedBlackboardKey)));

	const UObject* SelfObject = BlackboardComponent->GetValueAsObject(FName("SelfActor"));
	const AEnemy* SelfEnemy = Cast<AEnemy>(SelfObject);

	if (Character && SelfEnemy)
	{
		FHitResult HitResult;
		const TArray<AActor*> ActorsToIgnore;
		
		return !UKismetSystemLibrary::LineTraceSingle(GetWorld(), SelfEnemy->GetActorLocation(), Character->GetActorLocation(), ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green, FLinearColor::Red, 2.f);
	}

	return false;
}
