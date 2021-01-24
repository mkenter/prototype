// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Prototype/Characters/Abilities/AttributeSets/UPBaseAttributeSet.h"
#include "PCharacterBase.generated.h"

class APWeapon;
class UGameplayEffect;
class UPGameplayAbility;
class UPBaseAbilitySystemComponent;
class UAbilitySystemComponent;

UCLASS()
class PROTOTYPE_API APCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APCharacterBase(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitializeAttributesEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UPGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetSprintSpeedMultiplier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	UPBaseAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UPBaseAttributeSet* AttributeSetBase;

	virtual void InitializeAttributes();

	virtual void GiveAbilities(TArray<TSubclassOf<UPGameplayAbility>> AbilitySet);

	virtual void AddStartupEffects(TArray<TSubclassOf<class UGameplayEffect>> Effects);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|Inventory")
    virtual void EquipWeapon(APWeapon* NewWeapon);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	APWeapon* EquippedWeapon;

};
