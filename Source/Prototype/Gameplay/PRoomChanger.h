// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Prototype/PrototypeGameMode.h"
#include "GameFramework/Actor.h"
#include "PRoomChanger.generated.h"

class UBoxComponent;
UCLASS()
class PROTOTYPE_API APRoomChanger final : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APRoomChanger();

	UPROPERTY()
	APrototypeGameMode* GameMode;

	UFUNCTION()
	void UpdateRooms(int8 RoomId) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxComponentOneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxComponentTwoBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);


public:

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UBillboardComponent* BillboardComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* BoxComponentOne;

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* BoxComponentTwo;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	int8 BoxComponentOneRoomId;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	int8 BoxComponentTwoRoomId;
};
