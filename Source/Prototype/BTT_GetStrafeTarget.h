// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_GetStrafeTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UBTT_GetStrafeTarget final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	UBTT_GetStrafeTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
