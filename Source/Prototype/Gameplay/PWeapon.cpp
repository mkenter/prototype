// Fill out your copyright notice in the Description page of Project Settings.


#include "PWeapon.h"

#include "PrototypeProjectile.h"
#include "Camera/CameraComponent.h"
#include "Prototype/Characters/Player/PPlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Prototype/Characters/Abilities/PBaseAbilitySystemComponent.h"
#include "Prototype/Characters/Abilities/PGameplayAbility.h"

// Sets default values
APWeapon::APWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	bIsEquipped = false;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	AbilitySystemComponent = CreateDefaultSubobject<UPBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	MuzzleLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocationComponent"));
	MuzzleLocationComponent->SetupAttachment(SkeletalMeshComponent);
}

UAbilitySystemComponent* APWeapon::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void APWeapon::BeginPlay()
{
	Super::BeginPlay();

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &APWeapon::OnOverlapBegin);
}

void APWeapon::InitializeAttributes()
{
	if (AbilitySystemComponent && InitializeAttributesEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		AbilitySystemComponent->ApplyGameplayEffectToSelf(
			Cast<UGameplayEffect>(InitializeAttributesEffect->GetDefaultObject()), 1, EffectContext);
	}
}

void APWeapon::GiveAbilities(TArray<TSubclassOf<UPGameplayAbility>> AbilitySet)
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

void APWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
                              AActor* OtherActor,
                              UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex,
                              bool bFromSweep,
                              const FHitResult& SweepResult)
{
	if (bIsEquipped)
	{
		return;
	}

	APPlayerCharacter* Player = Cast<APPlayerCharacter>(OtherActor);

	if (Player)
	{
		bIsEquipped = true;
		Player->EquipWeapon(this);

		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->InitAbilityActorInfo(this, this);

			InitializeAttributes();
			GiveAbilities(DefaultAbilities);
		}

		OwningCharacter = Player;
	}
}

void APWeapon::FireProjectile()
{
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();

		if (World)
		{
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
                ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			APPlayerCharacter* Player = Cast<APPlayerCharacter>(OwningCharacter);

			if (Player)
			{
				// spawn the projectile at the muzzle
				APrototypeProjectile* NewProjectile = World->SpawnActor<APrototypeProjectile>(ProjectileClass, MuzzleLocationComponent->GetComponentLocation(), Player->FirstPersonCameraComponent->GetComponentRotation(), ActorSpawnParams);

				if (NewProjectile)
				{
					NewProjectile->SetSpeed(1000.f);
				}
			}
		}
	}
}
