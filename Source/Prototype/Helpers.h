// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/NoExportTypes.h"
#include "Helpers.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UHelpers : public UObject
{
	GENERATED_BODY()

public:

	static FCollisionQueryParams ConfigureCollisionParams(FName TraceTag, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf, UObject* WorldContextObject);

	static void DrawDebugLineTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, EDrawDebugTrace::Type DrawDebugType, bool bHit, const FHitResult& OutHit, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime);

	static void DrawDebugCapsuleTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, float Radius, float HalfHeight, EDrawDebugTrace::Type DrawDebugType, bool bHit, const FHitResult& OutHit, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime);
	
	static bool LineTraceSingle(UObject* WorldContextObject, const FVector Start, const FVector End, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime, const ECollisionChannel CollisionChannel);

	static bool SimpleLineTrace(UWorld* World, const FVector Start, const FVector End, FHitResult& HitResultOut, const bool bIsDebug);

	static bool CapsuleTraceSingle(UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime);

	static bool SimpleCapsuleTrace(UWorld* World, const FVector Start, const FVector End, float CapsuleRadius, float CapsuleHalfHeight, FHitResult& HitResultOut, bool bIsDebug);

	static bool OldLineTrace(UWorld* World, const FVector Start, const FVector End, FHitResult& HitResultOut, const bool bIsDebug, const float DebugLifeTime);
	
	static bool OldCapsuleTrace(UWorld* World, const FVector Start, const FVector End, float CapsuleRadius, float CapsuleHalfHeight, AActor* IgnoredActor, FHitResult& HitResultOut, bool bIsDebug, const float DebugLifetime);

	static FVector GetLocationAboveTarget(UWorld* World, const FVector TargetLocation, const float DistanceAbove);
	
};
