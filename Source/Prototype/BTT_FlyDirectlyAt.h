// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FlyingEnemy.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FlyDirectlyAt.generated.h"

class AEnemy;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UBTT_FlyDirectlyAt final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTT_FlyDirectlyAt();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY()
	ACharacter* TargetCharacter;

	UPROPERTY()
	AFlyingEnemy* Self;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float HeightOffset;
	
};
