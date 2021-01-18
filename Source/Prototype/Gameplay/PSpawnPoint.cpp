// Fill out your copyright notice in the Description page of Project Settings.


#include "PSpawnPoint.h"
#include "Prototype/Characters/Enemies/Enemy.h"

void APSpawnPoint::SpawnEnemy() const
{
	AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyToSpawn, GetActorLocation(), GetActorRotation());
	NewEnemy->RoomId = RoomId;
}
