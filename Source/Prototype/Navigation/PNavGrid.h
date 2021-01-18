// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PNavGrid.generated.h"

class UBoxComponent;
class UPNavNode;

struct FZStruct
{
	TArray<UPNavNode*> FGridArray;
};

struct FYStruct
{
	TArray<FZStruct*> SArray;
};

UCLASS()
class PROTOTYPE_API APNavGrid final : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY()
	FVector OriginLocation;

	UPROPERTY()
	FIntVector GridTileCount;

	UFUNCTION()
	FIntVector GetGridTileCount() const;

	UFUNCTION()
	void DrawDebug() const;
	
	TArray<FYStruct*> Grid;

	UFUNCTION()
    UPNavNode* NodeFromWorldPoint(FVector WorldPosition) const;

	TArray<UPNavNode*> GetNeighbors(UPNavNode* CurrentNode);

	UFUNCTION()
    static int GetDistance(UPNavNode* NodeA, UPNavNode* NodeB);

	UFUNCTION()
    TArray<UPNavNode*> RetracePath(UPNavNode* StartNode, UPNavNode* EndNode) const;

	UFUNCTION()
	void CreateGrid();
	
public:	
	// Sets default values for this actor's properties
	APNavGrid();

	UPROPERTY()
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float TileSize;

	UFUNCTION()
    TArray<UPNavNode*> FindPath(FVector StartPosition, FVector TargetPosition);

	UFUNCTION()
    void DrawPath(TArray<UPNavNode*> PathToDraw, float Duration) const;

	UPROPERTY(EditAnywhere, Category = "Grid")
	bool bDrawDebug;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
