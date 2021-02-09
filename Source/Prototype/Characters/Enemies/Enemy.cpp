// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Prototype/Characters/Abilities/PBaseAbilitySystemComponent.h"
#include "Prototype/Characters/Player/PPlayerCharacter.h"
#include "Prototype/UI/PEnemyFloatingWidget.h"
#include "Prototype/Gameplay/PWeapon.h"

AEnemy::AEnemy(const class FObjectInitializer& ObjectInitializer) : APCharacterBase(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	KeepAtDistance = 800.f;

	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Character");

	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));
	SenseConfig_Sight->SightRadius = 1000.f;
	SenseConfig_Sight->LoseSightRadius = 2000.f;
	SenseConfig_Sight->PeripheralVisionAngleDegrees = 45.f;
	SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComponent->ConfigureSense(*SenseConfig_Sight);
	PerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnPerceptionUpdated);

	AbilitySystemComponent = CreateDefaultSubobject<UPBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSetBase = CreateDefaultSubobject<UPBaseAttributeSet>(TEXT("AttributeSetBase"));

	FloatingWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingWidgetComponent"));
	FloatingWidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FloatingWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingWidgetComponent->SetDrawAtDesiredSize(true);
	FloatingWidgetComponent->SetVisibility(false);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	UPEnemyFloatingWidget* EnemyFloatingWidget = Cast<UPEnemyFloatingWidget>(FloatingWidgetComponent->GetUserWidgetObject());

	if (EnemyFloatingWidget)
	{
		EnemyFloatingWidget->SetOwningEnemy(this);
	}

	if (StartingWeapon)
	{
		const USkeletalMeshSocket* GripSocket = GetUsableMesh()->GetSocketByName("GripPoint");

		if (GripSocket)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
                ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
			APWeapon* SpawnedWeapon = GetWorld()->SpawnActor<APWeapon>(StartingWeapon, GetActorLocation(), FRotator(0.f), ActorSpawnParams);

			if (SpawnedWeapon)
			{
				SpawnedWeapon->OnEquip(this);
				EquipWeapon(SpawnedWeapon, GripSocket, GetUsableMesh());
			}
		}
	}
}

void AEnemy::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (CurrentTarget || !UpdatedActors.Num() || !IsAlive())
	{
		return;
	}

	for (AActor* Actor : UpdatedActors)
	{
		// APCharacterBase* PotentialTarget = Cast<APCharacterBase>(Actor);
		//
		// if (PotentialTarget)
		// {
		// 	AController* MyController = GetController();
		//
		// 	if (MyController)
		// 	{
		// 		AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(MyController);
		//
		// 		if (EnemyAIController)
		// 		{
		// 			CurrentTarget = PotentialTarget;
		// 			EnemyAIController->BlackboardComponent->SetValueAsObject(FName("Target"), PotentialTarget);
		// 			break;
		// 		}
		// 	}
		// }
		
		/**
		 * Casting to player character in lieu of teams for now
		 */
		APPlayerCharacter* Player = Cast<APPlayerCharacter>(Actor);
		
		if (Player)
		{
			/**
			 * Set target class property and BB value
			 */
			const bool bSetTarget = SetCurrentTarget(Player);

			if (bSetTarget)
			{
				break;
			}
		}
	}
}

void AEnemy::Die()
{
	Super::Die();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCurrentTarget(nullptr);
}

void AEnemy::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector CurrentLocation = GetActorLocation();
	const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (Player)
	{
		const FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation();

		const float Distance = (CurrentLocation - PlayerLocation).Size();

		if (!FloatingWidgetComponent)
		{
			return;
		}

		if (Distance <= 1200.f && !FloatingWidgetComponent->GetVisibleFlag())
		{
			FloatingWidgetComponent->SetVisibility(true);
		}

		if (Distance > 1200.f && FloatingWidgetComponent->GetVisibleFlag())
		{
			FloatingWidgetComponent->SetVisibility(false);
		}
	}
}

bool AEnemy::SetCurrentTarget(ACharacter* NewTarget)
{
	AController* MyController = GetController();

	if (MyController)
	{
		AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(MyController);

		if (EnemyAIController)
		{
			CurrentTarget = NewTarget;
			EnemyAIController->BlackboardComponent->SetValueAsObject(FName("Target"), NewTarget);
			
			return true;
		}
	}
	
	return false;
}
