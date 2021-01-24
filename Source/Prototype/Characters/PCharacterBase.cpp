// Fill out your copyright notice in the Description page of Project Settings.


#include "PCharacterBase.h"
#include "GameplayEffectTypes.h"
#include "Abilities/PBaseAbilitySystemComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Prototype/Characters/Abilities/PGameplayAbility.h"
#include "Prototype/Gameplay/PWeapon.h"

// Sets default values
APCharacterBase::APCharacterBase(const class FObjectInitializer& ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* APCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float APCharacterBase::GetMoveSpeed() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMoveSpeed();
	}

	return 0.f;
}

float APCharacterBase::GetSprintSpeedMultiplier()
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetSprintModifier();
	}

	return 0.f;
}

// Called when the game starts or when spawned
void APCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void APCharacterBase::InitializeAttributes()
{
	if (AbilitySystemComponent && InitializeAttributesEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		AbilitySystemComponent->ApplyGameplayEffectToSelf(
			Cast<UGameplayEffect>(InitializeAttributesEffect->GetDefaultObject()), 1, EffectContext);
	}
}

void APCharacterBase::GiveAbilities(TArray<TSubclassOf<UPGameplayAbility>> AbilitySet)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UPGameplayAbility>& StartupAbility : AbilitySet)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1,
			                                                         static_cast<int32>(StartupAbility.
				                                                         GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void APCharacterBase::AddStartupEffects(TArray<TSubclassOf<UGameplayEffect>> Effects)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (const TSubclassOf<UGameplayEffect> GameplayEffect : Effects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
			
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}
}

// Called every frame
void APCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		InitializeAttributes();
		GiveAbilities(DefaultAbilities);
		AddStartupEffects(StartupEffects);
	}
}

void APCharacterBase::EquipWeapon(APWeapon* NewWeapon)
{
	if (NewWeapon)
	{
		NewWeapon->SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		NewWeapon->SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		NewWeapon->SkeletalMeshComponent->SetSimulatePhysics(false);

		const USkeletalMeshSocket* GripSocket = MeshComponent->GetSocketByName("GripPoint");

		if (GripSocket)
		{
			NewWeapon->AttachToComponent(MeshComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}

		EquippedWeapon = NewWeapon;
	}	
}
