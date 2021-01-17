// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WaypointArray.generated.h"

class UNavNode;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UWaypointArray final : public UObject
{
	GENERATED_BODY()

public:

	TArray<UNavNode*> Path; 
	
};
