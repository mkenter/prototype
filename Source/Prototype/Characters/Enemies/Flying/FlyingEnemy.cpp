// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingEnemy.h"
#include "DrawDebugHelpers.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/CharacterMovementComponent.h"

AFlyingEnemy::AFlyingEnemy(const class FObjectInitializer& ObjectInitializer) : AEnemy(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	KeepAtDistance = 600.f;
	MovementComponent = GetCharacterMovement();
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
}

void AFlyingEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AFlyingEnemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AFlyingEnemy::FlyToTargetLocation(const FVector CurrentLocation, const FVector TargetLocation, const float DeltaTime, const float Speed)
{
	FHitResult* HitResult = nullptr;
	const float MaxSpeed = MovementComponent->GetMaxSpeed();
	const float Alpha = MaxSpeed / (CurrentLocation - TargetLocation).Size() * DeltaTime;
	const FVector DestinationLocation = FMath::Lerp<FVector>(CurrentLocation, TargetLocation, Alpha);

	SetActorLocation(DestinationLocation, true, HitResult, ETeleportType::TeleportPhysics);

	if (HitResult)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s - %s"), *DestinationLocation.ToString(), *HitResult->Location.ToString());
		return HitResult->bBlockingHit;
	}

	return false;
}
