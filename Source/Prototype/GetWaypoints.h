// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GetWaypoints.generated.h"

class AFlyingEnemy;
/**
 * 
 */
UCLASS()
class PROTOTYPE_API UGetWaypoints : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UGetWaypoints();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

