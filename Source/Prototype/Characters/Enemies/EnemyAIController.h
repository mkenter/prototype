// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AEnemyAIController final : public AAIController
{
	GENERATED_BODY()
	

public:

	explicit AEnemyAIController(const FObjectInitializer& ObjectInitializer);

	virtual void SetPawn(APawn* InPawn) override;

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(VisibleAnywhere, Category = "Team")
	FGenericTeamId TeamId;
	
};
