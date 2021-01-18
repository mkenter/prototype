// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "EnemyAIController.h"
#include "Prototype/Characters/Player/PrototypeCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	KeepAtDistance = 800.f;

	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));
	SenseConfig_Sight->SightRadius = 1000.f;
	SenseConfig_Sight->LoseSightRadius = 1200.f;
	SenseConfig_Sight->PeripheralVisionAngleDegrees = 45.f;
	SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComponent->ConfigureSense(*SenseConfig_Sight);
	PerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnPerceptionUpdated);

	CurrentHitpoints = 100.f;
	TotalHitpoints = 100.f;
}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                         AActor* DamageCauser)
{
	if (CurrentHitpoints > Damage)
	{
		CurrentHitpoints -= Damage;
		return Damage;
	}

	CurrentHitpoints = 0.f;
	Die();

	return CurrentHitpoints;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (CurrentTarget || !UpdatedActors.Num())
	{
		return;
	}

	for (AActor* Actor : UpdatedActors)
	{
		/**
		 * Casting to player character in lieu of teams for now
		 */
		APrototypeCharacter* Character = Cast<APrototypeCharacter>(Actor);

		if (Character)
		{
			/**
			 * Set target class property and BB value
			 */
			CurrentTarget = Character;

			AController* MyController = GetController();
			AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(MyController);

			if (EnemyAIController)
			{
				EnemyAIController->BlackboardComponent->SetValueAsObject(FName("Target"), Character);
			}

			break;
		}
	}
}

void AEnemy::Die()
{
	Destroy();
}
