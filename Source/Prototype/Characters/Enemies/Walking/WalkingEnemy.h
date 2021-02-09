// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prototype/Characters/Enemies/Enemy.h"
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

public:

	AWalkingEnemy(const class FObjectInitializer& ObjectInitializer);

	
private:
	
	UFUNCTION()
	static float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation);

	
protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetCurrentSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetCurrentDirection() const;

	UFUNCTION(BlueprintCallable, Category = "Targetting")
	bool CheckRange() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targetting")
	float ShootingRange;

	
public:
	
	virtual void PlayDeathAnimation() override;
	
	virtual void PlayFireRifleAnimation() override;
	
	virtual void PlayHitReactAnimation() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targetting")
	bool bUseIronSights;
};
