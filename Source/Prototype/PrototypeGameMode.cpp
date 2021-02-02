// Copyright Epic Games, Inc. All Rights Reserved.

#include "PrototypeGameMode.h"
#include "Characters/Enemies/Enemy.h"
#include "Characters/Player/PPlayerCharacter.h"
#include "UI/PrototypeHUD.h"
#include "Gameplay/PRoomChanger.h"
#include "Gameplay/PSpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Player/PPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Gameplay/PWeapon.h"
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APSpawnPoint::StaticClass(), PossibleSpawnPoints);

	for (AActor* CurrentActor : PossibleSpawnPoints)
	{
		APSpawnPoint* SpawnPoint = Cast<APSpawnPoint>(CurrentActor);

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
	TArray<AEnemy*> Enemies = GetAllEnemies();

	for (AEnemy* Enemy : Enemies)
	{
		if (Enemy->RoomId != RoomId)
		{
			Enemy->EquippedWeapon->Destroy();
			Enemy->Destroy();
		}
	}
}

void APrototypeGameMode::SpawnAdjacentRooms(int8 RoomId) const
{
	TArray<AActor*> PossibleRoomChangers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APRoomChanger::StaticClass(), PossibleRoomChangers);

	for (AActor* PossibleRoomChanger : PossibleRoomChangers)
	{
		APRoomChanger* RoomChanger = Cast<APRoomChanger>(PossibleRoomChanger);

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

void APrototypeGameMode::PlayerDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("APrototypeGameMode::PlayerDeath"));
	
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (Character)
	{
		APPlayerCharacter* Player = Cast<APPlayerCharacter>(Character);

		if (Player)
		{
			Player->DisableInput(nullptr);
			StopAllEnemies();
			OnPlayerDeath();

			// TODO: Figure out why this doesn't work
			Player->MeshComponent->GetAnimInstance()->Montage_Play(Player->Montage, 1.f);
			Player->DeathEnd();
		}
	}
}

TArray<AEnemy*> APrototypeGameMode::GetAllEnemies() const
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Actors);

	TArray<AEnemy*> Enemies;
	Enemies.Reserve(Actors.Num());

	for (AActor* CurrentActor : Actors)
	{
		AEnemy* Enemy = Cast<AEnemy>(CurrentActor);

		if (Enemy)
		{
			Enemies.Add(Enemy);
		}
	}

	return Enemies;
}

void APrototypeGameMode::StopAllEnemies() const
{
	TArray<AEnemy*> Enemies = GetAllEnemies();

	for (AEnemy* Enemy : Enemies)
	{
		Enemy->SetCurrentTarget(nullptr);
		Enemy->DisableInput(nullptr);
	}
}
