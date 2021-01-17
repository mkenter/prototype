// Copyright Epic Games, Inc. All Rights Reserved.

#include "PrototypeGameMode.h"
#include "Enemy.h"
#include "PrototypeHUD.h"
#include "PrototypeCharacter.h"
#include "RoomChanger.h"
#include "SpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "PPlayerController.h"
#include "UObject/ConstructorHelpers.h"

APrototypeGameMode::APrototypeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/Blueprints/BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<APPlayerController> PlayerControllerFinder(TEXT("/Game/Player/Blueprints/BP_PPlayerController"));
	PlayerControllerClass = PlayerControllerFinder.Class;

	// use our custom HUD class
	HUDClass = APrototypeHUD::StaticClass();

	CurrentRoomId = 0;
}

void APrototypeGameMode::BeginPlay()
{
	SpawnRoom(CurrentRoomId);
	SpawnAdjacentRooms(CurrentRoomId);
}

void APrototypeGameMode::SpawnRoom(const int8 RoomId) const
{
	TArray<AActor*> PossibleSpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), PossibleSpawnPoints);

	for (AActor* CurrentActor : PossibleSpawnPoints)
	{
		ASpawnPoint* SpawnPoint = Cast<ASpawnPoint>(CurrentActor);

		if (SpawnPoint && SpawnPoint->RoomId == RoomId)
		{
			SpawnPoint->SpawnEnemy();
		}
	}
}

void APrototypeGameMode::SetCurrentRoomId(const int8 NewCurrentRoomId)
{
	CurrentRoomId = NewCurrentRoomId;	
}

int8 APrototypeGameMode::GetCurrentRoomId() const
{
	return CurrentRoomId;
}

void APrototypeGameMode::DespawnAllButRoom(int8 RoomId) const
{
	TArray<AActor*> PossibleEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), PossibleEnemies);

	for (AActor* CurrentActor : PossibleEnemies)
	{
		AEnemy* Enemy = Cast<AEnemy>(CurrentActor);

		if (Enemy && Enemy->RoomId != RoomId)
		{
			Enemy->Destroy();
		}
	}
}

void APrototypeGameMode::SpawnAdjacentRooms(int8 RoomId) const
{
	TArray<AActor*> PossibleRoomChangers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoomChanger::StaticClass(), PossibleRoomChangers);

	for (AActor* PossibleRoomChanger : PossibleRoomChangers)
	{
		ARoomChanger* RoomChanger = Cast<ARoomChanger>(PossibleRoomChanger);

		if (RoomChanger)
		{
			if (RoomChanger->BoxComponentOneRoomId == RoomId)
			{
				SpawnRoom(RoomChanger->BoxComponentTwoRoomId);
			}

			if (RoomChanger->BoxComponentTwoRoomId == RoomId)
			{
				SpawnRoom(RoomChanger->BoxComponentOneRoomId);
			}
		}
	}
}
