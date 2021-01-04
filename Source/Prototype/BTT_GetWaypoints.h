// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_GetWaypoints.generated.h"

class AFlyingEnemy;
/**
 * 
 */
UCLASS()
class PROTOTYPE_API UBTT_GetWaypoints final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTT_GetWaypoints();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

