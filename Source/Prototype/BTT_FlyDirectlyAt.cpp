// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FlyDirectlyAt.h"

#include "Helpers.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_FlyDirectlyAt::UBTT_FlyDirectlyAt()
{
	NodeName = "Fly Directly At";
	bNotifyTick = true;
	bNotifyTaskFinished = true;

	HeightOffset = 0.f;
}

EBTNodeResult::Type UBTT_FlyDirectlyAt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const FName SelectedBlackboardKey = GetSelectedBlackboardKey();
	
	UObject* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelectedBlackboardKey);
	ACharacter* Character = Cast<ACharacter>(Target);

	if (Character)
	{
		TargetCharacter = Character;
	}
	
	UObject* SelfObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"));
	AFlyingEnemy* SelfEnemy = Cast<AFlyingEnemy>(SelfObject);

	if (SelfEnemy)
	{
		Self = SelfEnemy;
	}

	if (TargetCharacter && Self)
	{
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

void UBTT_FlyDirectlyAt::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, const float DeltaSeconds)
{
	const FVector SelfLocation = Self->GetActorLocation();
	FVector CurrentTargetLocation = TargetCharacter->GetActorLocation();

	if (HeightOffset)
	{
		CurrentTargetLocation = UHelpers::GetLocationAboveTarget(GetWorld(), TargetCharacter->GetActorLocation(), HeightOffset);
	}

	if ((SelfLocation - CurrentTargetLocation).Size() < 100.f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		Self->FlyToTargetLocation(SelfLocation, CurrentTargetLocation, DeltaSeconds, 0.5f);
	}
}
