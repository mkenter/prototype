// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class PROTOTYPE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	ACharacter* CurrentTarget;

	UPROPERTY()
	bool bHasLineToTarget;
	
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float KeepAtDistance;

	UFUNCTION(BlueprintCallable)
	void SetCurrentTarget(ACharacter* NewTarget)
	{
		CurrentTarget = NewTarget;
	}

	UFUNCTION(BlueprintCallable)
    AActor* GetCurrentTarget() const
	{
		return CurrentTarget;
	}

};
