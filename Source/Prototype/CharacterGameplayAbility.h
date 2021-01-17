// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prototype.h"
#include "Abilities/GameplayAbility.h"
#include "CharacterGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UCharacterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UCharacterGameplayAbility();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	EAbilitySystemInputID AbilityInputID = EAbilitySystemInputID::None;
	
};
