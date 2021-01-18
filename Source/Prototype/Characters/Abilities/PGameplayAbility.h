// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prototype/Prototype.h"
#include "Abilities/GameplayAbility.h"
#include "PGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UPGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPGameplayAbility();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	EAbilitySystemInputID AbilityInputID = EAbilitySystemInputID::None;
	
};
