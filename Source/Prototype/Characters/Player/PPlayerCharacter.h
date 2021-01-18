// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prototype/Characters/PCharacterBase.h"
#include "PPlayerCharacter.generated.h"

class USkeletalMeshComponent;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API APPlayerCharacter : public APCharacterBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:

	APPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;
};
