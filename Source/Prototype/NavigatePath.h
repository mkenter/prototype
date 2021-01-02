// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "NavigatePath.generated.h"

class UNavNode;
class AFlyingEnemy;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UNavigatePath final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	UNavigatePath();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY()
	TArray<UNavNode*> Path;

	UPROPERTY()
	UNavNode* TargetNode;

	UPROPERTY()
	AFlyingEnemy* Self;
};
