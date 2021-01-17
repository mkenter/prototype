// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavGrid.generated.h"

class UBoxComponent;
class UNavNode;

struct FZStruct
{
	TArray<UNavNode*> FGridArray;
};

struct FYStruct
{
	TArray<FZStruct*> SArray;
};

UCLASS()
class PROTOTYPE_API ANavGrid final : public AActor
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
    UNavNode* NodeFromWorldPoint(FVector WorldPosition) const;

	TArray<UNavNode*> GetNeighbors(UNavNode* CurrentNode);

	UFUNCTION()
    static int GetDistance(UNavNode* NodeA, UNavNode* NodeB);

	UFUNCTION()
    TArray<UNavNode*> RetracePath(UNavNode* StartNode, UNavNode* EndNode) const;

	UFUNCTION()
	void CreateGrid();
	
public:	
	// Sets default values for this actor's properties
	ANavGrid();

	UPROPERTY()
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float TileSize;

	UFUNCTION()
    TArray<UNavNode*> FindPath(FVector StartPosition, FVector TargetPosition);

	UFUNCTION()
    void DrawPath(TArray<UNavNode*> PathToDraw, float Duration) const;

	UPROPERTY(EditAnywhere, Category = "Grid")
	bool bDrawDebug;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
