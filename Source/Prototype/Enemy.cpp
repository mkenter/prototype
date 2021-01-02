// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	KeepAtDistance = 800.f;

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	CurrentTarget = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void AEnemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

