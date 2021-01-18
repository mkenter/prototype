// Fill out your copyright notice in the Description page of Project Settings.


#include "PGetWaypoints.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Prototype/Characters/Enemies/Flying/FlyingEnemy.h"
#include "Prototype/Navigation/PNavGrid.h"
#include "Prototype/Navigation/PNavNode.h"
#include "Prototype/Navigation/PWaypointArray.h"
#include "Kismet/GameplayStatics.h"

UPGetWaypoints::UPGetWaypoints()
{
	NodeName = "Get Waypoints";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UPGetWaypoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Execute UGetWaypoints"));
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	
	UObject* Target = BlackboardComponent->GetValueAsObject(FName("Target"));
	ACharacter* Character = Cast<ACharacter>(Target);

	UObject* SelfObject = BlackboardComponent->GetValueAsObject(FName("SelfActor"));
	AFlyingEnemy* SelfEnemy = Cast<AFlyingEnemy>(SelfObject);

	APNavGrid* ActiveNavGrid = nullptr;
	TArray<AActor*> PossibleNavGridActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APNavGrid::StaticClass(), PossibleNavGridActors);

	for (AActor* PossibleNavGridActor : PossibleNavGridActors)
	{
		APNavGrid* PossibleNavGrid = Cast<APNavGrid>(PossibleNavGridActor);

		if (PossibleNavGrid)
		{
			ActiveNavGrid = PossibleNavGrid;
		}
	}

	if (Character && SelfEnemy && ActiveNavGrid)
	{
		const TArray<UPNavNode*> Path = ActiveNavGrid->FindPath(SelfEnemy->GetActorLocation(), Character->GetActorLocation());
		UPWaypointArray* WaypointArray = NewObject<UPWaypointArray>();
		WaypointArray->Path = Path;
		ActiveNavGrid->DrawPath(Path, 20.f);

		BlackboardComponent->SetValueAsVector(FName("PathTargetLocation"), Character->GetActorLocation());

		BlackboardComponent->SetValueAsObject(FName("Path"), WaypointArray);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
