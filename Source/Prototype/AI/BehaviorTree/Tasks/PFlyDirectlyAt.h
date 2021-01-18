// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../../Characters/Enemies/Flying/FlyingEnemy.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "PFlyDirectlyAt.generated.h"

class AEnemy;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UPFlyDirectlyAt final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UPFlyDirectlyAt();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY()
	ACharacter* TargetCharacter;

	UPROPERTY()
	AFlyingEnemy* Self;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float HeightOffset;
	
};
