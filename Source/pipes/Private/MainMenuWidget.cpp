// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "GameLevelMode.h"

void UMainMenuWidget::NativeConstruct()
{
	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	}
	if (StartButton) {
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartClicked);
	}
}

void UMainMenuWidget::setOnStartClickedCallback(utils::ClickEventCallback const startCallback)
{
	mStartCallback = startCallback;
}

void UMainMenuWidget::setOnQuitClickedCallback(utils::ClickEventCallback const quitCallback)
{
	mQuitCallback = quitCallback;
}

void UMainMenuWidget::OnStartClicked()
{
	mStartCallback();
}

void UMainMenuWidget::OnQuitClicked()
{
	mQuitCallback();
}
