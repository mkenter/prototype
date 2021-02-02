// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FireAtWill.generated.h"

class AEnemy;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UFireAtWill final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UFireAtWill();

	UPROPERTY()
	ACharacter* TargetCharacter;

	UPROPERTY()
	AEnemy* Self;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
