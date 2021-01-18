// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "PGetStrafeTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UPGetStrafeTarget final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	UPGetStrafeTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
