// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Prototype/Characters/PCharacterBase.h"
#include "PPlayerCharacter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API APPlayerCharacter final : public APCharacterBase, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	APPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	

protected:

	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Die() override;

	// virtual void HealthChanged(const FOnAttributeChangeData& Data) override;

	
private:

	virtual FGenericTeamId GetGenericTeamId() const override;
	
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	virtual void PossessedBy(AController* NewController) override;

	virtual void EquipWeapon(APWeapon* NewWeapon, const USkeletalMeshSocket* GripSocket, USkeletalMeshComponent* SkeletalMeshComponent) override;

	virtual USkeletalMeshComponent* GetUsableMesh() const override;

	virtual void PlayDeathAnimation() override;
	
	virtual void PlayFireRifleAnimation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Team")
	FGenericTeamId TeamId;
};
