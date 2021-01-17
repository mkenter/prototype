// Fill out your copyright notice in the Description page of Project Settings.


#include "NavGrid.h"
#include "NavNode.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ANavGrid::ANavGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->InitBoxExtent(FVector(500.f, 500.f, 500.f));
	RootComponent = BoxComponent;

	TileSize = 50.f;
}

// Called when the game starts or when spawned
void ANavGrid::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OriginLocation = BoxComponent->GetComponentLocation() - BoxComponent->GetScaledBoxExtent();
	GridTileCount = GetGridTileCount();

	CreateGrid();
	
	if (bDrawDebug)
	{
		DrawDebug();
	}
}

// Called every frame
void ANavGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FIntVector ANavGrid::GetGridTileCount() const
{
	const FVector TileCountRaw = BoxComponent->GetScaledBoxExtent() / TileSize;
	return FIntVector(FMath::RoundToInt(TileCountRaw.X) - 1, FMath::RoundToInt(TileCountRaw.Y) - 1,
	                  FMath::RoundToInt(TileCountRaw.Z) - 1);
}

void ANavGrid::DrawDebug() const
{
	for (int X = 0; X < Grid.Num(); X++)
	{
		for (int Y = 0; Y < Grid[X]->SArray.Num(); Y++)
		{
			for (int Z = 0; Z < Grid[X]->SArray[Y]->FGridArray.Num(); Z++)
			{
				UNavNode* CurrentGridNode = Grid[X]->SArray[Y]->FGridArray[Z];
				FLinearColor DebugColor = FLinearColor(1.f, 0.f, 0.f, .6f);
				float DebugThickness = 2.f;

				if (CurrentGridNode->bWalkable)
				{
					DebugColor = FLinearColor(0.f, 1.f, 0.f, 0.2f);
					DebugThickness = 0.5f;
				}

				UKismetSystemLibrary::DrawDebugBox(GetWorld(), CurrentGridNode->WorldLocation, FVector(TileSize - 2.f),
				                                   DebugColor, FRotator(0.f), 1000.f, DebugThickness);
			}
		}
	}
}

void ANavGrid::DrawPath(TArray<UNavNode*> PathToDraw, float Duration) const
{
	if (PathToDraw.Num())
	{
		for (int i = 1; i < PathToDraw.Num(); i++)
		{
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), PathToDraw[i - 1]->WorldLocation,
			                                    PathToDraw[i]->WorldLocation, FLinearColor::Black, Duration, 2.f);
			UKismetSystemLibrary::DrawDebugPoint(GetWorld(), PathToDraw[i]->WorldLocation, 8.f, FLinearColor::Blue,
			                                     Duration);
		}
	}
}

UNavNode* ANavGrid::NodeFromWorldPoint(const FVector WorldPosition) const
{
	FVector const BoxExtent = BoxComponent->GetScaledBoxExtent();
	float const PercentX = FMath::Clamp<float>((WorldPosition.X - OriginLocation.X) / (BoxExtent.X * 2), 0, 1);
	float const PercentY = FMath::Clamp<float>((WorldPosition.Y - OriginLocation.Y) / (BoxExtent.Y * 2), 0, 1);
	float const PercentZ = FMath::Clamp<float>((WorldPosition.Z - OriginLocation.Z) / (BoxExtent.Z * 2), 0, 1);

	const int X = FMath::RoundToInt(GridTileCount.X * PercentX);
	const int Y = FMath::RoundToInt(GridTileCount.Y * PercentY);
	const int Z = FMath::RoundToInt(GridTileCount.Z * PercentZ);

	return Grid[X]->SArray[Y]->FGridArray[Z];
}

TArray<UNavNode*> ANavGrid::FindPath(const FVector StartPosition, const FVector TargetPosition)
{
	UNavNode* StartNode = NodeFromWorldPoint(StartPosition);
	UNavNode* TargetNode = NodeFromWorldPoint(TargetPosition);

	TArray<UNavNode*> OpenSet;
	TArray<UNavNode*> ClosedSet;

	auto Predicate = [](const UNavNode& NodeA, const UNavNode& NodeB)
	{
		return NodeA.GetFCost() < NodeB.GetFCost() || NodeA.GetFCost() == NodeB.GetFCost() && NodeA.HCost < NodeB.HCost;
	};

	OpenSet.HeapPush(StartNode, Predicate);

	while (OpenSet.Num() > 0)
	{
		UNavNode* CurrentNode;
		OpenSet.HeapPop(CurrentNode, Predicate);
		ClosedSet.Add(CurrentNode);

		if (CurrentNode == TargetNode)
		{
			return RetracePath(StartNode, TargetNode);
		}

		for (UNavNode* Neighbor : GetNeighbors(CurrentNode))
		{
			if (!Neighbor->bWalkable || ClosedSet.Contains(Neighbor))
			{
				continue;
			}

			const int NewMovementCostToNeighbor = CurrentNode->GCost + GetDistance(CurrentNode, Neighbor);

			if (NewMovementCostToNeighbor < Neighbor->GCost || !OpenSet.Contains(Neighbor))
			{
				Neighbor->GCost = NewMovementCostToNeighbor;
				Neighbor->HCost = GetDistance(Neighbor, TargetNode);
				Neighbor->ParentNode = CurrentNode;

				if (!OpenSet.Contains(Neighbor))
				{
					OpenSet.HeapPush(Neighbor, Predicate);
				}
			}
		}
	}
	return {};
}

TArray<UNavNode*> ANavGrid::GetNeighbors(UNavNode* CurrentNode)
{
	TArray<UNavNode*> Neighbors;

	for (int X = -1; X <= 1; X++)
	{
		for (int Y = -1; Y <= 1; Y++)
		{
			for (int Z = -1; Z <= 1; Z++)
			{
				if (Y == 0 && X == 0 && Z == 0)
				{
					continue;
				}

				const int CheckX = CurrentNode->GridLocation.X + X;
				const int CheckY = CurrentNode->GridLocation.Y + Y;
				const int CheckZ = CurrentNode->GridLocation.Z + Z;

				if (CheckX >= 0 && CheckX < GridTileCount.X && CheckY >= 0 && CheckY < GridTileCount.Y && CheckZ >= 0 &&
					CheckZ < GridTileCount.Z)
				{
					Neighbors.Add(Grid[CheckX]->SArray[CheckY]->FGridArray[CheckZ]);
				}
			}
		}
	}

	return Neighbors;
}

int ANavGrid::GetDistance(UNavNode* NodeA, UNavNode* NodeB)
{
	int OutputValue = 0;
	int SubtractionModifier = 0;
	TArray<int> Values;
	Values.Reserve(3);
	Values.Add(22);
	Values.Add(14);
	Values.Add(10);

	const int DistanceX = FMath::Abs(NodeA->GridLocation.X - NodeB->GridLocation.X);
	const int DistanceY = FMath::Abs(NodeA->GridLocation.Y - NodeB->GridLocation.Y);
	const int DistanceZ = FMath::Abs(NodeA->GridLocation.Z - NodeB->GridLocation.Z);

	TArray<int> Distances;
	Distances.Reserve(3);
	Distances.Add(DistanceX);
	Distances.Add(DistanceY);
	Distances.Add(DistanceZ);

	Distances.Sort([](const int DistanceA, const int DistanceB)
	{
		return DistanceA < DistanceB;
	});

	for (int i = 0; i < Distances.Num(); i++)
	{
		OutputValue += Values[i] * (Distances[i] - SubtractionModifier);
		SubtractionModifier += Distances[i];
	}

	return OutputValue;
}

TArray<UNavNode*> ANavGrid::RetracePath(UNavNode* StartNode, UNavNode* EndNode) const
{
	TArray<UNavNode*> BuiltPath;
	UNavNode* CurrentNode = EndNode;

	while (CurrentNode != StartNode)
	{
		BuiltPath.Add(CurrentNode);
		CurrentNode = CurrentNode->ParentNode;
	}

	BuiltPath.Add(StartNode);
	Algo::Reverse(BuiltPath);

	return BuiltPath;
}

void ANavGrid::CreateGrid()
{
	for (int X = 0; X <= GridTileCount.X; X++)
	{
		FYStruct* NewYGridArray = new FYStruct;

		for (int Y = 0; Y <= GridTileCount.Y; Y++)
		{
			FZStruct* NewZGridArray = new FZStruct;

			for (int Z = 0; Z <= GridTileCount.Z; Z++)
			{
				FVector ForwardVector = (TileSize * 2.f * X + TileSize) * BoxComponent->GetForwardVector();
				FVector RightVector = (TileSize * 2.f * Y + TileSize) * BoxComponent->GetRightVector();
				FVector UpVector = (TileSize * 2.f * Z + TileSize) * BoxComponent->GetUpVector();
				const FVector TilePosition = OriginLocation + ForwardVector + RightVector + UpVector;

				TArray<AActor*> ActorsToIgnore;
				FHitResult HitResult;
				UNavNode* NewNavNode = NewObject<UNavNode>(this);
				NewNavNode->TileSize = TileSize;
				NewNavNode->GridLocation = FIntVector(X, Y, Z);
				NewNavNode->WorldLocation = TilePosition;
				NewNavNode->bWalkable = !UKismetSystemLibrary::SphereTraceSingle(
					GetWorld(), TilePosition, TilePosition, TileSize, ETraceTypeQuery::TraceTypeQuery3, false,
					ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green,
					0.f);

				NewZGridArray->FGridArray.Add(NewNavNode);
			}

			NewYGridArray->SArray.Add(NewZGridArray);
		}

		Grid.Add(NewYGridArray);
	}
}
