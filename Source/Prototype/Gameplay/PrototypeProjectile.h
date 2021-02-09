// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrototypeProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UMeshComponent;
class APCharacterBase;

UCLASS(config=Game)
class APrototypeProjectile : public AActor
{
	GENERATED_BODY()

public:
	APrototypeProjectile();


private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;


protected:
	virtual void BeginPlay() override;


public:
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(EditAnywhere, Category = "Stats")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float InitialSpeed;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxSpeed;

	UFUNCTION(BlueprintImplementableEvent, Category = "Hit")
	void OnHitEvent(APCharacterBase* HitCharacter);
};

