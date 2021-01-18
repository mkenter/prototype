// Fill out your copyright notice in the Description page of Project Settings.


#include "PGetStrafeTarget.h"


#include "../../../Characters/Enemies/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UPGetStrafeTarget::UPGetStrafeTarget()
{
	NodeName = "Get Strafe Target";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UPGetStrafeTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Execute UGetStrafeTarget"));
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	UObject* Target = BlackboardComponent->GetValueAsObject(FName("Target"));
	ACharacter* Character = Cast<ACharacter>(Target);

	UObject* SelfObject = BlackboardComponent->GetValueAsObject(FName("SelfActor"));
	AEnemy* SelfEnemy = Cast<AEnemy>(SelfObject);

	if (Character && SelfEnemy)
	{
		/**
		 * Calculate right and left FVectors as degrees around target
		 */
		const UCapsuleComponent* CapsuleComponent = SelfEnemy->GetCapsuleComponent();
		const FVector CurrentLocation = SelfEnemy->GetActorLocation();
		const FVector CharacterLocation = Character->GetActorLocation();
		
		const float Distance = (CurrentLocation - CharacterLocation).Size();
		const float DistanceModifer = 800.f / Distance;
		const float RandomAngle = FMath::RandRange(8.f * DistanceModifer, 16.f * DistanceModifer);
		
		const FVector NormalizedVector = (CurrentLocation - CharacterLocation).GetSafeNormal(1.f);
		const FVector LeftRotatedVector = NormalizedVector.RotateAngleAxis(RandomAngle, FVector(0.f, 0.f, 1.f));
		const FVector RightRotatedVector = NormalizedVector.RotateAngleAxis(RandomAngle * -1, FVector(0.f, 0.f, 1.f));
		FVector LeftVector = LeftRotatedVector * Distance + CharacterLocation;
		FVector RightVector = RightRotatedVector * Distance + CharacterLocation;

		/**
		 * Trace to the target vectors and stop short as needed
		 */
		FHitResult LeftHitResult;
		FHitResult RightHitResult;
		TArray<AActor*> ActorsToIgnore;
		
		const bool LeftTraceHit = UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), CurrentLocation, LeftVector, CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetScaledCapsuleHalfHeight(), ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, LeftHitResult, true, FLinearColor::Green, FLinearColor::Red, 2.f);

		if (LeftTraceHit)
		{
			LeftVector = LeftHitResult.Location;
		}

		const bool RightTraceHit = UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), CurrentLocation, RightVector, CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetScaledCapsuleHalfHeight(), ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, RightHitResult, true, FLinearColor::Green, FLinearColor::Red, 2.f);

		if (RightTraceHit)
		{
			RightVector = RightHitResult.Location;
		}

		FHitResult LeftSightHitResult;
		FHitResult RightSightHitResult;

		const bool LeftSightTraceHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), LeftVector, CharacterLocation, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, LeftSightHitResult, true, FLinearColor::Green, FLinearColor::Red, 2.f);

		const bool RightSightTraceHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), RightVector, CharacterLocation, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, RightSightHitResult, true, FLinearColor::Green, FLinearColor::Red, 2.f);

		TArray<FVector> PossibleLocations;
		PossibleLocations.Reserve(2);
		
		if (!LeftSightTraceHit)
		{
			PossibleLocations.Add(LeftVector);
		}
		
		if (!RightSightTraceHit)
		{
			PossibleLocations.Add(RightVector);
		}
		
		if (PossibleLocations.Num() == 0)
		{
			BlackboardComponent->SetValueAsBool(FName("bShouldStrafe"), false);
			return EBTNodeResult::Succeeded;
		}
		
		if (PossibleLocations.Num() == 1)
		{
			BlackboardComponent->SetValueAsVector(FName("StrafeTargetLocation"), PossibleLocations[0]);
		}
		else
		{
			int32 RandomIndex = FMath::RandRange(0,1 );
			BlackboardComponent->SetValueAsVector(FName("StrafeTargetLocation"), PossibleLocations[RandomIndex]);
		}

		BlackboardComponent->SetValueAsBool(FName("bShouldStrafe"), true);

		UKismetSystemLibrary::DrawDebugPoint(GetWorld(), BlackboardComponent->GetValueAsVector(FName("StrafeTargetLocation")), 16.f, FLinearColor::Blue, 10.f);
		
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
