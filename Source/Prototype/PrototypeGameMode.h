// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PrototypeGameMode.generated.h"

class UWidgetComponent;
class AEnemy;

UCLASS(minimalapi)
class APrototypeGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
	int8 CurrentRoomId;

	UPROPERTY()
	int8 NextRoomId;

protected:
	virtual void BeginPlay() override;

public:
	APrototypeGameMode();

	UFUNCTION()
    void SpawnRoom(int8 RoomId) const;

	UFUNCTION()
	void SetCurrentRoomId(int8 NewCurrentRoomId);

	UFUNCTION()
    int8 GetCurrentRoomId() const;

	UFUNCTION()
	void DespawnAllButRoom(int8 RoomId) const;

	UFUNCTION()
	void SpawnAdjacentRooms(int8 RoomId) const;

	UFUNCTION()
	void PlayerDeath();

	UFUNCTION()
	TArray<AEnemy*> GetAllEnemies() const;
	
	UFUNCTION()
	void StopAllEnemies() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnPlayerDeath();
};



