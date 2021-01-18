// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "PSpawnPoint.generated.h"

class AEnemy;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API APSpawnPoint final : public ATargetPoint
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AEnemy> EnemyToSpawn;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	int8 RoomId;

	UFUNCTION()
	void SpawnEnemy() const;
	
};
