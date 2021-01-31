// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prototype/Characters/Enemies/Enemy.h"
#include "FlyingEnemy.generated.h"

class UBlackboardData;
class UBehaviorTree;
class ATargetPoint;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AFlyingEnemy final : public AEnemy
{
	GENERATED_BODY()
	
public:

	AFlyingEnemy(const class FObjectInitializer& ObjectInitializer);

	
protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


public:
	
	UFUNCTION()
    bool FlyToTargetLocation(FVector CurrentLocation, FVector TargetLocation, float DeltaTime, float Speed);

	
};
