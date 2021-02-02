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

	APCharacterBase(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitializeAttributesEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UPGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetSprintSpeedMultiplier();

protected:

	virtual void BeginPlay() override;

	UPROPERTY()
	UPBaseAttributeSet* AttributeSetBase;

	virtual void InitializeAttributes();

	virtual void GiveAbilities(TArray<TSubclassOf<UPGameplayAbility>> AbilitySet);

	virtual void AddStartupEffects(TArray<TSubclassOf<class UGameplayEffect>> Effects);

	UFUNCTION()
    virtual void Die();

	FDelegateHandle HealthChangedDelegateHandle;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	
	FGameplayTag DeadTag;

	virtual bool IsAlive() const;
	

public:
	
	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	UPBaseAbilitySystemComponent* AbilitySystemComponent;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|Inventory")
	virtual void EquipWeapon(APWeapon* NewWeapon, const USkeletalMeshSocket* GripSocket, USkeletalMeshComponent* SkeletalMeshComponent);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	APWeapon* EquippedWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* Montage;

	UFUNCTION(BlueprintCallable)
	virtual void DeathEnd();

	UFUNCTION(BlueprintCallable, Category = "Mesh")
	virtual USkeletalMeshComponent* GetUsableMesh() const;

};
