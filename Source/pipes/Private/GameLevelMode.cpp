// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLevelMode.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "InGameMouseController.h"
#include "Blueprint/UserWidget.h"

AGameLevelMode::AGameLevelMode()
	: mInitilized(false)
	, mCurrentState(GameState::gameStart)
{
	/// Suppress spawning default pawn
	DefaultPawnClass = nullptr;
	PlayerControllerClass = AInGameMouseController::StaticClass();
}

bool AGameLevelMode::handleNewState() {
	UE_LOG(LogTemp, Error, TEXT("Handling game state"));
	bool status(false);
	switch (mCurrentState)
	{
	case GameState::mainMenu: {
		/// reset game session
		/// level generator generates buffer if necessary
		/// display mainMenu

		status = !!showMainMenu();
		break;
	}
	case GameState::inGame: {
		/// start session
		/// hide mainMenu
		/// spawn gameBoard
		/// set level
		if (mMainMenuWidget) {
			mMainMenuWidget->RemoveFromParent();
		}

		mSpawnedBoard = spawnGameBoard();
		status = !!mSpawnedBoard;
		break;
	}
	case GameState::pauseMenu: {
		/// stop timers
		/// display pauseMenu
		status = true;
		break;
	}
	case GameState::scoreBoard: {
		/// hide mainMenu
		/// display scoreBoard
		status = true;
		break;
	}
	default:
		break;
	}
	return status;
}

bool AGameLevelMode::showMainMenu()
{
	bool status(false);

	if (IsValid(mMainMenuWidgetClass)) {
		mMainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), mMainMenuWidgetClass);
		if (mMainMenuWidget) {
			mMainMenuWidget->setOnStartClickedCallback([this]() {
				setGameState(GameState::inGame);
				});
			mMainMenuWidget->setOnQuitClickedCallback([this]() {
				GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
				});

			mMainMenuWidget->AddToViewport(1);
			status = true;
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Main menu widget not created"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("mMainMenuWidgetClass is nullptr"));
	}

	return status;
}

AGameBoardActor* AGameLevelMode::spawnGameBoard()
{

	if (!mSpawnedBoard) {
		UE_LOG(LogTemp, Error, TEXT("Spawned board is nullptr"));
		return nullptr;
	}

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);

	AGameBoardActor* gameBoard;
	gameBoard = GetWorld()->SpawnActor<AGameBoardActor>(mSpawnedBoard->StaticClass(), Location, Rotation);
	if (!gameBoard) {
		UE_LOG(LogTemp, Error, TEXT("Tilemap not loaded"));
		return nullptr;
	}

	// Currently we only set a 10x10 gameBoard to test functionality
	if (!gameBoard->init(10, 10)) {
		UE_LOG(LogTemp, Error, TEXT("Couldn't initialize game board"));
	};
	gameBoard->setDemoState();

	FVector CameraLocation(150.0f, 75.0f, -150.0f);
	FRotator CameraRotation(0.0f, 270.0f, 0.0f); // Y, Z, X for some reason
	mCamera = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation);
	mCamera->SetOwner(this);

	auto camera = mCamera->GetCameraComponent();
	camera->ProjectionMode = ECameraProjectionMode::Orthographic;
	camera->SetOrthoWidth(750);

	auto controller = GetWorld()->GetFirstPlayerController();
	if (controller && (mCamera->HasActiveCameraComponent())) {
		controller->SetViewTarget(mCamera);
	}
	else {
		return nullptr;
	}

	return gameBoard;
}

void AGameLevelMode::setGameState(GameState const gameState) {
	UE_LOG(LogTemp, Error, TEXT("Setting game state"));
	mCurrentState = gameState;

	if (!handleNewState()) {
		UE_LOG(LogTemp, Error, TEXT("Failed setting state: %u"), gameState);
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
		/// log and handle error
		return;
	}
}

bool AGameLevelMode::initAllObjects() {
	if (mInitilized) {
		/// log this, it shoudn't happen
		return false;
	}

	mMainMenuWidgetClass = LoadClass<UMainMenuWidget>(this, TEXT("/Game/Menus/MainMenuWidgetBP.MainMenuWidgetBP_C"));
	mSpawnedBoard = NewObject<AGameBoardActor>(this);

	mInitilized = (mMainMenuWidgetClass && mSpawnedBoard);

	/// init level generator
	/// init score manager
	/// init game board (pop one layout from level generator)
	/// log and return after each step
	/// 
	return mInitilized;
}

void AGameLevelMode::BeginPlay()
{
	if (!initAllObjects()) {
		/// log and exit
		exit(-1);
	}

	setGameState(GameState::mainMenu);
}
