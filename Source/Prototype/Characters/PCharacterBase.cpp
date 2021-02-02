// Fill out your copyright notice in the Description page of Project Settings.


#include "PCharacterBase.h"
#include "GameplayEffectTypes.h"
#include "Abilities/PBaseAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Prototype/Characters/Abilities/PGameplayAbility.h"
#include "Prototype/Gameplay/PWeapon.h"

APCharacterBase::APCharacterBase(const class FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Character");
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* APCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* APCharacterBase::GetAttributeSet() const
{
	return AttributeSetBase;
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

void APCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &APCharacterBase::HealthChanged);
	}
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

void APCharacterBase::Die()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	
	if (Montage)
	{
		UAnimInstance* AnimInstance = GetUsableMesh()->GetAnimInstance();
		
		if (AnimInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("APCharacterBase::Die"));
			AnimInstance->Montage_Play(Montage, 1.f);
			AnimInstance->Montage_JumpToSection(FName("Death"), Montage);
		}
	}
}

void APCharacterBase::HealthChanged(const FOnAttributeChangeData& Data)
{
	
	const float Health = Data.NewValue;

	if (Health <= 0 && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		Die();
	}
}

void APCharacterBase::DeathEnd()
{
	USkeletalMeshComponent* SkeletalMesh = GetUsableMesh();
	SkeletalMesh->bPauseAnims = true;
	SkeletalMesh->bNoSkeletonUpdate = true;
}

USkeletalMeshComponent* APCharacterBase::GetUsableMesh() const
{
	return GetMesh();
}

bool APCharacterBase::IsAlive() const
{
	return !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag);
}

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

void APCharacterBase::EquipWeapon(APWeapon* NewWeapon, const USkeletalMeshSocket* GripSocket, USkeletalMeshComponent* SkeletalMeshComponent)
{
	if (NewWeapon)
	{
		NewWeapon->SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		NewWeapon->SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		NewWeapon->SkeletalMeshComponent->SetSimulatePhysics(false);

		if (GripSocket)
		{
			GripSocket->AttachActor(NewWeapon, SkeletalMeshComponent);
			// NewWeapon->AttachToComponent(GetUsableMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
			EquippedWeapon = NewWeapon;
		}
	}	
}
