// Fill out your copyright notice in the Description page of Project Settings.


#include "Prototype/UI/PEnemyFloatingWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPEnemyFloatingWidget::SetOwningEnemy(AEnemy* InEnemy)
{
	OwningEnemy = InEnemy;
}

void UPEnemyFloatingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!OwningEnemy)
	{
		return;
	}

	UAttributeSet* AS = OwningEnemy->GetAttributeSet();

	if (AS)
	{
		UPBaseAttributeSet* AttributeSet = Cast<UPBaseAttributeSet>(AS);

		if (AttributeSet)
		{
			FNumberFormattingOptions Opts;
			Opts.SetMaximumFractionalDigits(0);
	
			HealthBar->SetPercent(AttributeSet->GetHealth() / AttributeSet->GetMaxHealth());
			CurrentHealthText->SetText(FText::AsNumber(AttributeSet->GetHealth(), &Opts));
			MaxHealthText->SetText(FText::AsNumber(AttributeSet->GetMaxHealth(), &Opts));
		}
	}
}
