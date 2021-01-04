// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AEnemyAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	/**
	 * Run behavior tree assigned to enemy
	 */
	AEnemy* Enemy = Cast<AEnemy>(InPawn);
	
	if (Enemy)
	{
		if (Enemy->BehaviorTree)
		{
			RunBehaviorTree(Enemy->BehaviorTree);
		}

		if (Enemy->BlackboardData)
		{
			UseBlackboard(Enemy->BlackboardData, BlackboardComponent);
		}
	}
}
