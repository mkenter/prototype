// Fill out your copyright notice in the Description page of Project Settings.


#include "GetWaypoints.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "FlyingEnemy.h"
#include "NavGrid.h"
#include "NavNode.h"
#include "WaypointArray.h"
#include "Kismet/GameplayStatics.h"

UGetWaypoints::UGetWaypoints()
{
	NodeName = "Get Waypoints";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UGetWaypoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Execute UGetWaypoints"));
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	
	UObject* Target = BlackboardComponent->GetValueAsObject(FName("Target"));
	ACharacter* Character = Cast<ACharacter>(Target);

	UObject* SelfObject = BlackboardComponent->GetValueAsObject(FName("SelfActor"));
	AFlyingEnemy* SelfEnemy = Cast<AFlyingEnemy>(SelfObject);

	ANavGrid* ActiveNavGrid = nullptr;
	TArray<AActor*> PossibleNavGridActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavGrid::StaticClass(), PossibleNavGridActors);

	for (AActor* PossibleNavGridActor : PossibleNavGridActors)
	{
		ANavGrid* PossibleNavGrid = Cast<ANavGrid>(PossibleNavGridActor);

		if (PossibleNavGrid)
		{
			ActiveNavGrid = PossibleNavGrid;
		}
	}

	if (Character && SelfEnemy && ActiveNavGrid)
	{
		const TArray<UNavNode*> Path = ActiveNavGrid->FindPath(SelfEnemy->GetActorLocation(), Character->GetActorLocation());
		UWaypointArray* WaypointArray = NewObject<UWaypointArray>();
		WaypointArray->Path = Path;
		ActiveNavGrid->DrawPath(Path, 20.f);

		BlackboardComponent->SetValueAsVector(FName("PathTargetLocation"), Character->GetActorLocation());

		BlackboardComponent->SetValueAsObject(FName("Path"), WaypointArray);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
