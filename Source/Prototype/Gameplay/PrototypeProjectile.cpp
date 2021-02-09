// Copyright Epic Games, Inc. All Rights Reserved.

#include "PrototypeProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Prototype/Characters/PCharacterBase.h"

APrototypeProjectile::APrototypeProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &APrototypeProjectile::OnHit);
	// set up a notification for when this component hits something blocking
	// CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// CollisionComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	// CollisionComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;
	
	// Stats
	InitialSpeed = 3000.f;
	MaxSpeed = 3000.f;
	Damage = 60.f;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void APrototypeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* InstigatorActor = GetInstigator();

	if (InstigatorActor)
	{
		CollisionComp->IgnoreActorWhenMoving(InstigatorActor, true);
	}
}

void APrototypeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		APCharacterBase* Character = Cast<APCharacterBase>(OtherActor);

		if (Character)
		{
			OnHitEvent(Character);
			Character->PlayHitReactAnimation();
		}
	}

	Destroy();
}
