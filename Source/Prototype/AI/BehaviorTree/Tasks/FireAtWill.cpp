// Fill out your copyright notice in the Description page of Project Settings.


#include "Prototype/AI/BehaviorTree/Tasks/FireAtWill.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Prototype/Characters/Enemies/Enemy.h"

UFireAtWill::UFireAtWill()
{
	NodeName = "Fire At Will";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UFireAtWill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const FName SelectedBlackboardKey = GetSelectedBlackboardKey();
	
	UObject* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelectedBlackboardKey);
	ACharacter* Character = Cast<ACharacter>(Target);

	if (Character)
	{
		TargetCharacter = Character;
	}
	
	UObject* SelfObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"));
	AEnemy* SelfEnemy = Cast<AEnemy>(SelfObject);

	if (SelfEnemy)
	{
		Self = SelfEnemy;
	}

	if (TargetCharacter && Self)
	{
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

void UFireAtWill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UAbilitySystemComponent* AbilitySystemComponent = Self->GetAbilitySystemComponent();
	TArray<FGameplayAbilitySpec> Abilities = AbilitySystemComponent->GetActivatableAbilities();

	const bool bShot = AbilitySystemComponent->TryActivateAbility(Abilities[0].Handle);
	
	if (bShot)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
}
