// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NavNode.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UNavNode : public UObject
{
	GENERATED_BODY()

	
public:

	UPROPERTY(VisibleAnywhere, Category = "Node")
	float TileSize;

	UPROPERTY(VisibleAnywhere, Category = "Node")
	bool bWalkable;

	UPROPERTY(VisibleAnywhere, Category = "Node")
	int GCost;

	UPROPERTY(VisibleAnywhere, Category = "Node")
	int HCost;

	UPROPERTY(VisibleAnywhere, Category = "Node")
	FIntVector GridLocation;

	UPROPERTY(VisibleAnywhere, Category = "Node")
	UNavNode* ParentNode;

	UPROPERTY(VisibleAnywhere, Category = "Node")
	FVector WorldLocation;
	
	UFUNCTION()
    int GetFCost() const;
	
	
};
