// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "Enemy.h"

void ASpawnPoint::SpawnEnemy() const
{
	AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyToSpawn, GetActorLocation(), GetActorRotation());
	NewEnemy->RoomId = RoomId;
}
