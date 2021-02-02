// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Prototype/Characters/PCharacterBase.h"
#include "Enemy.generated.h"

class UPUserWidget;
class UWidgetComponent;
class AEnemyAIController;
class UBlackboardData;
class UAISenseConfig_Sight;
class UBehaviorTree;
class UAIPerceptionComponent;
class UAbilitySystemComponent;

UCLASS()
class PROTOTYPE_API AEnemy : public APCharacterBase
{
	GENERATED_BODY()

public:
	
	AEnemy(const class FObjectInitializer& ObjectInitializer);


protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    virtual void Die() override;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	ACharacter* CurrentTarget;

	UPROPERTY()
	bool bHasLineToTarget;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;


public:

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float KeepAtDistance;

	UFUNCTION(BlueprintCallable, Category = "Targetting")
	bool SetCurrentTarget(ACharacter* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "Targetting")
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

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	int8 RoomId;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	UWidgetComponent* FloatingWidgetComponent;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UClass* StartingWeapon;
};
