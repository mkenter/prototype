// Fill out your copyright notice in the Description page of Project Settings.


#include "PCapsuleCheck.h"
#include "../../../Helpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Prototype/Characters/Player/PrototypeCharacter.h"

UPCapsuleCheck::UPCapsuleCheck()
{
	NodeName = "Capsule Check";

	bNotifyBecomeRelevant = true;
	bNotifyTick = true;

	bAllowAbortLowerPri = true;
	bAllowAbortNone = true;
	bAllowAbortChildNodes = true;
	FlowAbortMode = EBTFlowAbortMode::None;

	HeightOffset = 0.f;
}

bool UPCapsuleCheck::CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// UE_LOG(LogTemp, Warning, TEXT("CalculateRawConditionValue"));
	
	const FName SelectedBlackboardKey = GetSelectedBlackboardKey();
	UObject* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelectedBlackboardKey);
	const APrototypeCharacter* TargetCharacter = Cast<APrototypeCharacter>(Target);

	const UObject* SelfObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"));
	const ACharacter* SelfCharacter = Cast<ACharacter>(SelfObject);

	if (TargetCharacter && SelfCharacter)
	{
		FHitResult HitResult;
		
		FVector CurrentTargetLocation = TargetCharacter->GetActorLocation();
		UCapsuleComponent* CapsuleComponent = SelfCharacter->GetCapsuleComponent();

		if (HeightOffset)
		{
			CurrentTargetLocation = UHelpers::GetLocationAboveTarget(GetWorld(), TargetCharacter->GetActorLocation(), HeightOffset);
		}

		return !UHelpers::SimpleCapsuleTrace(GetWorld(), SelfCharacter->GetActorLocation(), CurrentTargetLocation, CapsuleComponent->GetUnscaledCapsuleRadius(), CapsuleComponent->GetUnscaledCapsuleHalfHeight(), HitResult, true);
	}

	return false;
}
