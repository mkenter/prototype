// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayEffect.h"
#include "Prototype/Characters/Abilities/PBaseAbilitySystemComponent.h"
#include "Prototype/Characters/Abilities/PGameplayAbility.h"
#include "PBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UPBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
    static UPGameplayAbility* GetPrimaryAbilityInstanceFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
    static bool IsAbilitySpecHandleValid(FGameplayAbilitySpecHandle Handle);
	
};
