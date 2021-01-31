// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GameFramework/Actor.h"
#include "Niagara/Classes/NiagaraSystem.h"
#include "PWeapon.generated.h"

class APPlayerCharacter;
class APrototypeProjectile;
class APCharacterBase;
class UGameplayEffect;
class UPBaseAbilitySystemComponent;
class UPGameplayAbility;
class USkeletalMeshComponent;

UCLASS()
class PROTOTYPE_API APWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitializeAttributesEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UPGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsEquipped;

	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	UPBaseAbilitySystemComponent* AbilitySystemComponent;

	virtual void InitializeAttributes();
	
	virtual void GiveAbilities(TArray<TSubclassOf<UPGameplayAbility>> AbilitySet);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
	class USphereComponent* CollisionVolume;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Muzzle")
	USceneComponent* MuzzleLocationComponent;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Owner")
	APCharacterBase* OwningCharacter;

	UFUNCTION(BlueprintCallable, Category = "Shooting")
	void FireProjectile();

	UFUNCTION(BlueprintCallable, Category = "Shooting")
    ACharacter* FireHitScan();

	void SpawnFireEffects(const APPlayerCharacter* Player) const;

	void SpawnHitEffects(FHitResult HitResult) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting")
	UClass* ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting")
	UNiagaraSystem* FireEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting")
	UNiagaraSystem* HitEffects;

	UFUNCTION()
	void OnEquip(APCharacterBase* NewOwner);
};
