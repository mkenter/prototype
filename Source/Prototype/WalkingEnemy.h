// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "WalkingEnemy.generated.h"

class APrototypeProjectile;
class USceneComponent;
class USkeletalMeshComponent;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AWalkingEnemy : public AEnemy
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USceneComponent* MuzzleLocation;

	UFUNCTION()
	static float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation);

	UFUNCTION()
	void ResetWeaponCooldown();
	
	UPROPERTY()
	bool bWeaponOnCooldown;

	FTimerHandle WeaponTimerHandle;

	
protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void FireWeapon();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetCurrentSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetCurrentDirection() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool GetIsInRange() const;

	
public:

	AWalkingEnemy();

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<APrototypeProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	FVector GunOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float MaxFiringRange;

};
