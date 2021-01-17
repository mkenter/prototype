// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "PrototypeProjectile.h"

AWalkingEnemy::AWalkingEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("GripHand"));

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(WeaponMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.25f, 59.f, 11.3f));

	RateOfFire = 1.f;
	MaxFiringRange = 1000.f;
	bWeaponOnCooldown = false;
}

void AWalkingEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AWalkingEnemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTarget && !bWeaponOnCooldown)
	{
		FireWeapon();
	}

}

float AWalkingEnemy::GetCurrentSpeed() const
{
	return GetVelocity().Size();
}

float AWalkingEnemy::GetCurrentDirection() const
{
	return CalculateDirection(GetVelocity(), GetActorRotation());
}

bool AWalkingEnemy::GetIsInRange() const
{
	if (!CurrentTarget || (CurrentTarget->GetActorLocation() - GetActorLocation()).Size() > MaxFiringRange)
	{
		return false;
	}

	return true;
}

void AWalkingEnemy::ResetWeaponCooldown()
{
	bWeaponOnCooldown = false;
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

void AWalkingEnemy::FireWeapon()
{
	bWeaponOnCooldown = true;
	
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();

		if (World)
		{
			const FRotator SpawnRotation = GetControlRotation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			APrototypeProjectile* NewProjectile = World->SpawnActor<APrototypeProjectile>(ProjectileClass, MuzzleLocation->GetComponentLocation(), SpawnRotation, ActorSpawnParams);

			if (NewProjectile)
			{
				NewProjectile->SetSpeed(1000.f);
			}
		
			GetWorldTimerManager().SetTimer(WeaponTimerHandle, this, &AWalkingEnemy::ResetWeaponCooldown, RateOfFire);
		}
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}
