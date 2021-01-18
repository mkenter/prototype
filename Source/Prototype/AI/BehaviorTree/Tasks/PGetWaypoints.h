// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "PGetWaypoints.generated.h"

class AFlyingEnemy;
/**
 * 
 */
UCLASS()
class PROTOTYPE_API UPGetWaypoints final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UPGetWaypoints();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

