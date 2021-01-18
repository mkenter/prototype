// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class AEnemyAIController;
class UBlackboardData;
class UAISenseConfig_Sight;
class UBehaviorTree;
class UAIPerceptionComponent;
class UAbilitySystemComponent;

UCLASS()
class PROTOTYPE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	ACharacter* CurrentTarget;

	UPROPERTY()
	bool bHasLineToTarget;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	void Die();


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

	UPROPERTY(EditAnywhere)
	UAIPerceptionComponent* PerceptionComponent;

	UPROPERTY(EditAnywhere)
	UAISenseConfig_Sight* SenseConfig_Sight;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBlackboardData* BlackboardData;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float CurrentHitpoints;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float TotalHitpoints;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	int8 RoomId;

};
