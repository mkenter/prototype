// Fill out your copyright notice in the Description page of Project Settings.


#include "PCharacterMovementComponent.h"
#include "Prototype/Characters/PCharacterBase.h"

float UPCharacterMovementComponent::GetMaxSpeed() const
{
	APCharacterBase* Owner = Cast<APCharacterBase>(GetOwner());

	if (!Owner)
	{
		// UE_LOG(LogTemp, Warning, TEXT("UPCharacterMovementComponent::GetMaxSpeed No Owner"));
		return Super::GetMaxSpeed();
	}

	if (RequestToStartSprinting)
	{
		// UE_LOG(LogTemp, Warning, TEXT("UPCharacterMovementComponent::GetMaxSpeed Sprinting %f"), Owner->GetMoveSpeed() * Owner->GetSprintSpeedMultiplier());
		return Owner->GetMoveSpeed() * Owner->GetSprintSpeedMultiplier();
	}

	// UE_LOG(LogTemp, Warning, TEXT("UPCharacterMovementComponent::GetMaxSpeed Normal %f"), Owner->GetMoveSpeed());
	return Owner->GetMoveSpeed();
}

void UPCharacterMovementComponent::StartSprinting()
{
	RequestToStartSprinting = true;
}

void UPCharacterMovementComponent::StopSprinting()
{
	RequestToStartSprinting = false;
}
