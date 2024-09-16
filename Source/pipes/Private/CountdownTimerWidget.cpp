// Fill out your copyright notice in the Description page of Project Settings.

#include "CountdownTimerWidget.h"
#include "GameLevelMode.h"
#include "Components/TextBlock.h"

void UCountdownTimerWidget::NativeConstruct()
{
	if (CountDownText) {
		CountDownText->SetText(FText::FromString("Carefull! Overflow incoming!"));
	}
}

void UCountdownTimerWidget::TimerTick()
{
	if (mRemainingTime == 0.0) {
		GetWorld()->GetTimerManager().ClearTimer(mCountdownTimerHandle);
		CountDownText->SetText(FText::FromString("Overflow incoming!"));
		auto gameMode = Cast<AGameLevelMode>(GetWorld()->GetAuthGameMode());
		gameMode->setGameState(AGameLevelMode::GameState::Chasing);
		return;
	}

	FString val = FString::Format(TEXT("Remaining time: {0} seconds"), { mRemainingTime });

	CountDownText->SetText(FText::FromString(val));
	--mRemainingTime;
}

void UCountdownTimerWidget::startCountdown(int const time)
{
	mRemainingTime = time;

	if (CountDownText) {
		GetWorld()->GetTimerManager().SetTimer(mCountdownTimerHandle, this, &UCountdownTimerWidget::TimerTick, 1.0f, true);
	}
}
