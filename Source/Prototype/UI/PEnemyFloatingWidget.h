// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Prototype/Characters/Enemies/Enemy.h"
#include "Prototype/UI/PUserWidget.h"
#include "PEnemyFloatingWidget.generated.h"

class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS(Abstract)
class PROTOTYPE_API UPEnemyFloatingWidget : public UPUserWidget
{
	GENERATED_BODY()

protected:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
	AEnemy* OwningEnemy;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CurrentHealthText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MaxHealthText;

public:

	void SetOwningEnemy(AEnemy* InEnemy);
	
};
