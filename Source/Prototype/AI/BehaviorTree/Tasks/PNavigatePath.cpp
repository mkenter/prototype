// Fill out your copyright notice in the Description page of Project Settings.


#include "PNavigatePath.h"
#include "Prototype/Characters/Enemies/Flying/FlyingEnemy.h"
#include "Prototype/Navigation/PNavNode.h"
#include "Prototype/Navigation/PWaypointArray.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UPNavigatePath::UPNavigatePath()
{
	NodeName = "Navigate Path";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UPNavigatePath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	UObject* PathObject = BlackboardComponent->GetValueAsObject(FName("Path"));
	UPWaypointArray* WaypointArray = Cast<UPWaypointArray>(PathObject);

	UObject* SelfObject = BlackboardComponent->GetValueAsObject(FName("SelfActor"));
	AFlyingEnemy* SelfEnemy = Cast<AFlyingEnemy>(SelfObject);

	if (WaypointArray && WaypointArray->Path.Num() && SelfEnemy)
	{
		Path = WaypointArray->Path;
		Algo::Reverse(Path);
		Self = SelfEnemy;
		return EBTNodeResult::InProgress;
	}

	UE_LOG(LogTemp, Warning, TEXT("UBTT_NavigatePath::ExecuteTask Failed"));

	return EBTNodeResult::Failed;
}

void UPNavigatePath::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, const float DeltaSeconds)
{
	const FVector SelfLocation = Self->GetActorLocation();

	if (!TargetNode && Path.Num())
	{
		TargetNode = Path.Pop();
		UKismetSystemLibrary::DrawDebugPoint(GetWorld(), TargetNode->WorldLocation, 16.f,
		                                     FLinearColor(1.f, 0.f, 0.f, 1.f), 20.f);
	}

	if ((SelfLocation - TargetNode->WorldLocation).Size() < 100.f)
	{
		if (Path.Num())
		{
			TargetNode = Path.Pop();
			UKismetSystemLibrary::DrawDebugPoint(GetWorld(), TargetNode->WorldLocation, 16.f,
			                                     FLinearColor(1.f, 0.f, 0.f, 1.f), 20.f);
			Self->FlyToTargetLocation(SelfLocation, TargetNode->WorldLocation, DeltaSeconds, 1.f);
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else
	{
		Self->FlyToTargetLocation(SelfLocation, TargetNode->WorldLocation, DeltaSeconds, 1.f);
	}
}
