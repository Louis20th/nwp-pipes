// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameBoardActor.h"
#include "MainMenuWidget.h"

#include "GameLevelMode.generated.h"


UCLASS()
class PIPES_API AGameLevelMode : public AGameModeBase
{
	GENERATED_BODY()

	enum class GameState {
		gameStart,
		mainMenu,
		inGame,
		pauseMenu,
		scoreBoard
	};

public:
	AGameLevelMode();

	void BeginPlay() override;

	void setGameState(GameState const gameState);

private:
	bool initAllObjects();
	bool handleNewState();
	bool showMainMenu();
	bool spawnGameBoard();

	TSubclassOf<AGameBoardActor> mGameBoardActorClass;
	AGameBoardActor* mSpawnedBoard;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> mMainMenuWidgetClass;

	UPROPERTY()
	UMainMenuWidget* mMainMenuWidget;

	bool mInitilized;
	GameState mCurrentState;
};

