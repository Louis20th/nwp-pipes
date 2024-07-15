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
	// Inits all necessary objects
	// Returns true if successful
	bool initAllObjects();

	// Handles new state set by setGameState
	// Returns true if successful
	bool handleNewState();

	// Adds mainMenu to Viewport
	// Hard set at ZOrder 1
	// Returns true if successful
	bool showMainMenu();

	// Spawns gameBoardActor
	// Notice: Ensure that level is set correctly before spawning
	// Returns newly-spawned gameBoard
	AGameBoardActor* spawnGameBoard();

	TSubclassOf<AGameBoardActor> mGameBoardActorClass;
	AGameBoardActor* mSpawnedBoard;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> mMainMenuWidgetClass;

	UPROPERTY()
	UMainMenuWidget* mMainMenuWidget;

	bool mInitilized;
	GameState mCurrentState;
};

