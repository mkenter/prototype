// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "Sprint")
	uint8 RequestToStartSprinting : 1;

	virtual float GetMaxSpeed() const override;

	UFUNCTION(BlueprintCallable, Category = "Sprint")
    void StartSprinting();
	
	UFUNCTION(BlueprintCallable, Category = "Sprint")
    void StopSprinting();
	
};
