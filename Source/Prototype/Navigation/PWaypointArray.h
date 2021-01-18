// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PWaypointArray.generated.h"

class UPNavNode;

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UPWaypointArray final : public UObject
{
	GENERATED_BODY()

public:

	TArray<UPNavNode*> Path; 
	
};
