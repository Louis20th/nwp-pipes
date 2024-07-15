// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameBoardActor.h"
#include "MainMenuWidget.h"

#include "GameLevelMode.generated.h"

namespace core {
	enum class GameState {
		gameStart,
		mainMenu,
		inGame,
		pauseMenu,
		scoreBoard
	};
}

UCLASS()
class PIPES_API AGameLevelMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameLevelMode();

	void BeginPlay() override;

	void setGameState(core::GameState const gameState);

private:
	bool initAllObjects();
	bool handleNewState();

	TSubclassOf<AGameBoardActor> mGameBoardActorClass;
	AGameBoardActor* mSpawnedBoard;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> mMainMenuWidgetClass;

	UPROPERTY()
	UMainMenuWidget* mMainMenuWidget;

	bool mInitilized;
	core::GameState mCurrentState;
};

