// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingEnemy.h"
#include "Kismet/GameplayStatics.h"

AWalkingEnemy::AWalkingEnemy(const class FObjectInitializer& ObjectInitializer) : AEnemy(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	ShootingRange = 2000.f;
}

void AWalkingEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AWalkingEnemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	bUseIronSights = CheckRange();
}

float AWalkingEnemy::GetCurrentSpeed() const
{
	return GetVelocity().Size();
}

float AWalkingEnemy::GetCurrentDirection() const
{
	return CalculateDirection(GetVelocity(), GetActorRotation());
}

bool AWalkingEnemy::CheckRange() const
{
	if (CurrentTarget)
	{
		const FVector CurrentTargetLocation = CurrentTarget->GetActorLocation();
		const FVector MyLocation = GetActorLocation();
		const float Distance = (CurrentTargetLocation - MyLocation).Size();

		return Distance < ShootingRange;
	}
	
	return false;
}

void AWalkingEnemy::PlayDeathAnimation()
{
	PlayMontageSection(FName("Death"));
}

void AWalkingEnemy::PlayFireRifleAnimation()
{
	if (bUseIronSights)
	{
		PlayMontageSection(FName("FireRifleIronSights"));
		return;
	}
	
	PlayMontageSection(FName("FireRifle"));
}

void AWalkingEnemy::PlayHitReactAnimation()
{
	PlayMontageSection(FName("HitReact"));
}

float AWalkingEnemy::CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation)
{
	if (!Velocity.IsNearlyZero())
	{
		const FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		const FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		const FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		const FVector NormalizedVel = Velocity.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		const float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		const float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
		
		if (RightCosAngle < 0)
		{
			ForwardDeltaDegree *= -1;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}
