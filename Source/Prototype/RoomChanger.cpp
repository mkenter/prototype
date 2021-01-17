// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomChanger.h"
#include "PrototypeCharacter.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ARoomChanger::ARoomChanger()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	BillboardComponent->SetupAttachment(RootComponent);
	
	BoxComponentOne = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentOne"));
	BoxComponentOne->InitBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxComponentOne->SetRelativeTransform(FTransform(FVector(100.f, 0.f, 0.f)));
	BoxComponentOne->SetupAttachment(RootComponent);
	
	BoxComponentTwo = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentTwo"));
	BoxComponentTwo->InitBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxComponentTwo->SetRelativeTransform(FTransform(FVector(-100.f, 0.f, 0.f)));
	BoxComponentTwo->SetupAttachment(RootComponent);
}

void ARoomChanger::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase* GMode = GetWorld()->GetAuthGameMode();
	GameMode = Cast<APrototypeGameMode>(GMode);
	
	BoxComponentOne->OnComponentBeginOverlap.AddDynamic(this, &ARoomChanger::OnBoxComponentOneBeginOverlap);
	BoxComponentTwo->OnComponentBeginOverlap.AddDynamic(this, &ARoomChanger::OnBoxComponentTwoBeginOverlap);
}

void ARoomChanger::OnBoxComponentOneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APrototypeCharacter* Char = Cast<APrototypeCharacter>(OtherActor);

	if (Char)
	{
		UpdateRooms(BoxComponentOneRoomId);
	}
}

void ARoomChanger::OnBoxComponentTwoBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APrototypeCharacter* Char = Cast<APrototypeCharacter>(OtherActor);

	if (Char)
	{
		UpdateRooms(BoxComponentTwoRoomId);
	}
}


void ARoomChanger::UpdateRooms(const int8 RoomId) const
{
	if (RoomId != GameMode->GetCurrentRoomId())
	{
		GameMode->SetCurrentRoomId(RoomId);
		GameMode->DespawnAllButRoom(RoomId);
		GameMode->SpawnAdjacentRooms(RoomId);
	}
}
