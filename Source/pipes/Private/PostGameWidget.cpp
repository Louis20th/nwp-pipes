// Fill out your copyright notice in the Description page of Project Settings.


#include "PostGameWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "GameLevelMode.h"

void UPostGameWidget::onEnd(ChaseStatus const status, uint32 const score)
{
    FString val = FString::Format(TEXT("Score: {0}"), { score });
    ScoreText->SetText(FText::FromString(val));
    FText menuTxt;

    if (status == ChaseStatus::Won) {
        LeftButtonText->SetText(FText::FromString("Continue"));
        menuTxt = FText::FromString("Victory!");
    }
    else {
        LeftButtonText->SetText(FText::FromString("Retry"));
        menuTxt = FText::FromString("Defeat!");
    }
    MenuText->SetText(menuTxt);
}

void UPostGameWidget::NativeConstruct()
{
    if (QuitButtonText) {
        QuitButtonText->SetText(FText::FromString("Quit"));
    }

    if (QuitButton) {
        QuitButton->OnClicked.AddDynamic(this, &UPostGameWidget::onQuit);
    }

    if (LeftButton) {
        LeftButton->OnClicked.AddDynamic(this, &UPostGameWidget::onRestart);
    }
}

void UPostGameWidget::onQuit()
{
    auto gameMode = Cast<AGameLevelMode>(GetWorld()->GetAuthGameMode());
    gameMode->setGameState(AGameLevelMode::GameState::MainMenu);
}

void UPostGameWidget::onRestart()
{
    auto gameMode = Cast<AGameLevelMode>(GetWorld()->GetAuthGameMode());
    gameMode->setGameState(AGameLevelMode::GameState::Countdown);
}
