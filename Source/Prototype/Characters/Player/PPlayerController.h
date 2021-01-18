// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PPlayerController.generated.h"

class UPUserWidget;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API APPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UPUserWidget> UIHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UPUserWidget* UIHUDWidget;

public:

	void CreateHUD();
	
};
