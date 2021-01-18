// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers.h"
#include "DrawDebugHelpers.h"
#include "Gameplay/PrototypeProjectile.h"
#include "PhysicsEngine/PhysicsSettings.h"

FCollisionQueryParams UHelpers::ConfigureCollisionParams(FName TraceTag, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf, UObject* WorldContextObject)
{
	FCollisionQueryParams Params(TraceTag, SCENE_QUERY_STAT_ONLY(KismetTraceUtils), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.bReturnFaceIndex = !UPhysicsSettings::Get()->bSuppressFaceRemapTable; // Ask for face index, as long as we didn't disable globally
	Params.AddIgnoredActors(ActorsToIgnore);
	if (bIgnoreSelf)
	{
		AActor* IgnoreActor = Cast<AActor>(WorldContextObject);
		if (IgnoreActor)
		{
			Params.AddIgnoredActor(IgnoreActor);
		}
		else
		{
			// find owner
			UObject* CurrentObject = WorldContextObject;
			while (CurrentObject)
			{
				CurrentObject = CurrentObject->GetOuter();
				IgnoreActor = Cast<AActor>(CurrentObject);
				if (IgnoreActor)
				{
					Params.AddIgnoredActor(IgnoreActor);
					break;
				}
			}
		}
	}

	return Params;
}

void UHelpers::DrawDebugLineTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, const EDrawDebugTrace::Type DrawDebugType, const bool bHit, const FHitResult& OutHit, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		const bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		const float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		// @fixme, draw line with thickness = 2.f?
		if (bHit && OutHit.bBlockingHit)
		{
			// Red up to the blocking hit, green thereafter
			::DrawDebugLine(World, Start, OutHit.ImpactPoint, TraceColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugLine(World, OutHit.ImpactPoint, End, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugPoint(World, OutHit.ImpactPoint, 16.f, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}
		else
		{
			// no hit means all red
			::DrawDebugLine(World, Start, End, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}
	}
}

void UHelpers::DrawDebugCapsuleTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, const float Radius, const float HalfHeight, const EDrawDebugTrace::Type DrawDebugType, const bool bHit, const FHitResult& OutHit, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		const bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		const float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		if (bHit && OutHit.bBlockingHit)
		{
			// Red up to the blocking hit, green thereafter
			::DrawDebugCapsule(World, Start, HalfHeight, Radius, FQuat::Identity, TraceColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugCapsule(World, OutHit.Location, HalfHeight, Radius, FQuat::Identity, TraceColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugLine(World, Start, OutHit.Location, TraceColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugPoint(World, OutHit.ImpactPoint, 16.f, TraceColor.ToFColor(true), bPersistent, LifeTime);

			::DrawDebugCapsule(World, End, HalfHeight, Radius, FQuat::Identity, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugLine(World, OutHit.Location, End, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
		}
		else
		{
			// no hit means all red
			::DrawDebugCapsule(World, Start, HalfHeight, Radius, FQuat::Identity, TraceColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugCapsule(World, End, HalfHeight, Radius, FQuat::Identity, TraceColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugLine(World, Start, End, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}
	}
}

bool UHelpers::LineTraceSingle(UObject* WorldContextObject, const FVector Start, const FVector End, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
	const ECollisionChannel CollisionChannel = ECC_Visibility;

	static const FName LineTraceSingleName(TEXT("LineTraceSingle"));
	const FCollisionQueryParams Params = ConfigureCollisionParams(LineTraceSingleName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, WorldContextObject);

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	bool const bHit = World ? World->LineTraceSingleByChannel(OutHit, Start, End, CollisionChannel, Params) : false;

#if ENABLE_DRAW_DEBUG
	DrawDebugLineTraceSingle(World, Start, End, DrawDebugType, bHit, OutHit, TraceColor, TraceHitColor, DrawTime);
#endif

	return bHit;
}

bool UHelpers::SimpleLineTrace(UWorld* World, const FVector Start, const FVector End, FHitResult& HitResultOut, const bool bIsDebug)
{
	const TArray<AActor*> ActorsToIgnore;
	return LineTraceSingle(World, Start, End, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResultOut, true, FLinearColor(0, 255, 0, 0), FLinearColor(255, 0, 0, 0), 0.f);
}

bool UHelpers::OldLineTrace(UWorld* World, const FVector Start, const FVector End, FHitResult& HitResultOut, const bool bIsDebug, const float DebugLifeTime)
{
	const FCollisionQueryParams CollisionQueryParams;

	const bool bTraceHit = World->LineTraceSingleByChannel(HitResultOut, Start, End, ECC_Visibility, CollisionQueryParams);

	if (bIsDebug)
	{
		if (bTraceHit)
		{
			DrawDebugLine(World, Start, HitResultOut.Location, FColor::Green, false, DebugLifeTime);
			DrawDebugPoint(World, HitResultOut.Location, 16.f, FColor::Red, false, DebugLifeTime);
			DrawDebugLine(World, HitResultOut.Location, End, FColor::Red, false, DebugLifeTime);
		}
		else
		{
			DrawDebugLine(World, Start, End, FColor::Green, false, DebugLifeTime);
		}
	}
	
	return bTraceHit;
}

bool UHelpers::CapsuleTraceSingle(UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const float HalfHeight, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
	const ECollisionChannel CollisionChannel = ECC_Visibility;

	static const FName CapsuleTraceSingleName(TEXT("CapsuleTraceSingle"));
	const FCollisionQueryParams Params = ConfigureCollisionParams(CapsuleTraceSingleName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, WorldContextObject);

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	bool const bHit = World ? World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, CollisionChannel, FCollisionShape::MakeCapsule(Radius, HalfHeight), Params) : false;

#if ENABLE_DRAW_DEBUG
	DrawDebugCapsuleTraceSingle(World, Start, End, Radius, HalfHeight, DrawDebugType, bHit, OutHit, TraceColor, TraceHitColor, DrawTime);
#endif

	return bHit;
}

bool UHelpers::SimpleCapsuleTrace(UWorld* World, const FVector Start, const FVector End, const float CapsuleRadius, float CapsuleHalfHeight, FHitResult& HitResultOut, bool bIsDebug)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Reserve(10);
	
	return CapsuleTraceSingle(World, Start, End, CapsuleRadius, CapsuleHalfHeight, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResultOut, true, FLinearColor(0, 255, 0, 0), FLinearColor(255, 0, 0, 0), 0.f);
}

bool UHelpers::OldCapsuleTrace(UWorld* World, const FVector Start, const FVector End, float CapsuleRadius, float CapsuleHalfHeight, AActor* IgnoredActor, FHitResult& HitResultOut, bool bIsDebug, const float DebugLifetime)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(IgnoredActor);
	
	const bool bTraceHit = World->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight), CollisionQueryParams);

	if (bIsDebug)
	{
		if (bTraceHit)
		{
			DrawDebugCapsule(World, Start, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, FColor::Cyan, false, DebugLifetime);
			DrawDebugCapsule(World, HitResult.Location, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, FColor::Green, false, DebugLifetime);
			DrawDebugLine(World, Start, HitResult.Location, FColor::Green, false, DebugLifetime);

			DrawDebugCapsule(World, End, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, FColor::Red, false, DebugLifetime);
			DrawDebugLine(World, HitResult.Location, End, FColor::Red, false, DebugLifetime);
			
		}
		else
		{
			DrawDebugCapsule(World, Start, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, FColor::Green, false, 0.f);
			DrawDebugCapsule(World, End, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, FColor::Green, false, 0.f);
			DrawDebugLine(World, Start, End, FColor::Green, false, 0.f);
		}
	}

	return bTraceHit;
}

FVector UHelpers::GetLocationAboveTarget(UWorld* World, const FVector TargetLocation, const float DistanceAbove)
{
	const FVector AboveCurrentTargetLocation = TargetLocation + FVector(0.f, 0.f, DistanceAbove);

	FHitResult TraceUpHitResult;
	const bool bTraceUpHit = SimpleLineTrace(World, TargetLocation, AboveCurrentTargetLocation, TraceUpHitResult, false);

	if (bTraceUpHit)
	{
		return TraceUpHitResult.Location;
	}

	return AboveCurrentTargetLocation;
}