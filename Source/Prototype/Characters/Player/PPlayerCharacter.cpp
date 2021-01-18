// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Prototype/Characters/Player/PCharacterMovementComponent.h"
#include "Prototype/Characters/Abilities/PBaseAbilitySystemComponent.h"

void APPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APPlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APPlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APPlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APPlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(InputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APPlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APPlayerCharacter::LookUpAtRate);
	
}

APPlayerCharacter::APPlayerCharacter(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPCharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCameraComponent"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-40.f, 1.75f, 64.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetOnlyOwnerSee(true);
	MeshComponent->SetupAttachment(FirstPersonCameraComponent);
	MeshComponent->bCastDynamicShadow = false;
	MeshComponent->CastShadow = false;
	MeshComponent->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	MeshComponent->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	AbilitySystemComponent = CreateDefaultSubobject<UPBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSetBase = CreateDefaultSubobject<UPBaseAttributeSet>(TEXT("AttributeSetBase"));
}
