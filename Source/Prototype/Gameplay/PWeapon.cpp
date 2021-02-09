// Fill out your copyright notice in the Description page of Project Settings.


#include "PWeapon.h"
#include "DrawDebugHelpers.h"
#include "PrototypeProjectile.h"
#include "Camera/CameraComponent.h"
#include "Prototype/Characters/Player/PPlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Prototype/Helpers.h"
#include "Prototype/Characters/Abilities/PBaseAbilitySystemComponent.h"
#include "Prototype/Characters/Abilities/PGameplayAbility.h"
#include "NiagaraFunctionLibrary.h"
#include "Prototype/Characters/Enemies/Enemy.h"
#include "Prototype/Characters/Enemies/Walking/WalkingEnemy.h"

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

	Damage = 20.f;
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
		const USkeletalMeshSocket* GripSocket = Player->MeshComponent->GetSocketByName("GripPoint");

		if (GripSocket)
		{
			OnEquip(Player);
			Player->EquipWeapon(this, GripSocket, Player->MeshComponent);
		}
	}
}

void APWeapon::FireProjectile()
{
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();

		if (World)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ActorSpawnParams.Instigator = OwningCharacter;
			ActorSpawnParams.Owner = OwningCharacter;

			APPlayerCharacter* Player = Cast<APPlayerCharacter>(OwningCharacter);
			AEnemy* Enemy = Cast<AEnemy>(OwningCharacter);

			if (Player)
			{
				World->SpawnActor<APrototypeProjectile>(ProjectileClass, MuzzleLocationComponent->GetComponentLocation(), Player->FirstPersonCameraComponent->GetComponentRotation(), ActorSpawnParams);
				
			}
			else if (Enemy)
			{
				World->SpawnActor<APrototypeProjectile>(ProjectileClass, MuzzleLocationComponent->GetComponentLocation(), Enemy->GetActorRotation(), ActorSpawnParams);
			}

			OwningCharacter->PlayFireRifleAnimation();
		}
	}
}

ACharacter* APWeapon::FireHitScan()
{
	UWorld* const World = GetWorld();

	if (World)
	{
		FHitResult HitResult;
		const TArray<AActor*> ActorsToIgnore;
		
		const FVector MuzzleLocation = MuzzleLocationComponent->GetComponentLocation();
		FVector TraceEndLocation = FVector(0.f);
		FRotator Rotation = FRotator(0.f);
		
		const APPlayerCharacter* Player = Cast<APPlayerCharacter>(OwningCharacter);

		if (Player)
		{
			TraceEndLocation = MuzzleLocation + Player->FirstPersonCameraComponent->GetForwardVector() * 3000.f;
			Rotation = Player->FirstPersonCameraComponent->GetComponentRotation();
		}
		else
		{
			TraceEndLocation = MuzzleLocation + OwningCharacter->GetActorForwardVector() * 3000.f;
			Rotation = OwningCharacter->GetActorRotation();
		}

		if (Rotation != FRotator(0.f) && TraceEndLocation != FVector(0.f))
		{
			const bool bHit = UHelpers::LineTraceSingle(World, MuzzleLocation, TraceEndLocation, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FLinearColor::Green, FLinearColor::Red, 1.f, ECC_GameTraceChannel1);

			SpawnFireEffects(Rotation);
			SpawnHitEffects(HitResult);
			
			if (bHit)
			{
				ACharacter* Char = Cast<ACharacter>(HitResult.GetActor());

				if (Char)
				{
					return Char;
				}
			}
		}
	}

	return nullptr;
}

void APWeapon::SpawnFireEffects(const FRotator Rotation) const
{
	if (FireEffects)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireEffects, MuzzleLocationComponent->GetComponentLocation(), Rotation, FVector(1.f), true, true, ENCPoolMethod::None, true);
	}
}

void APWeapon::SpawnHitEffects(FHitResult HitResult) const
{
	// TODO: Work with the HitResult.ImpactNormal.Rotation() value to orient the effects instead of using FRotator(0.f)
	
	if (HitEffects)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffects, HitResult.Location, FRotator(0.f), FVector(1.f), true, true, ENCPoolMethod::None, true);
	}
}

void APWeapon::OnEquip(APCharacterBase* NewOwner)
{
	bIsEquipped = true;
	OwningCharacter = NewOwner;
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		InitializeAttributes();
		GiveAbilities(DefaultAbilities);
	}
}

