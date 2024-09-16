// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLevelMode.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "InGameMouseController.h"
#include "BoardLayout.h"
#include "Blueprint/UserWidget.h"

AGameLevelMode::AGameLevelMode()
	: mSession()
	, mCamera(nullptr)
	, mCurrentState(GameState::GameStarted)
	, mChaser()
	, mInitilized(false)
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
	case GameState::MainMenu: {
		if (mSession.getState() != SessionState::NotStarted) {
			mSession.setState(SessionState::NotStarted);
		}
		/// reset game session
		/// level generator generates buffer if necessary
		/// display mainMenu

		status = !!showMainMenu();
		break;
	}
	case GameState::Countdown: {
		// start session
		mSession.setState(SessionState::Started);

		// hide mainMenu
		if (mMainMenuWidget) {
			mMainMenuWidget->RemoveFromParent();
		}

		// spawn gameboard
		mSpawnedBoard = spawnGameBoard();

		status = mSpawnedBoard && startCountdown();
		break;
	}
	case GameState::Chasing: {
		mChaser.init(mSpawnedBoard->getLayout());
		mChaser.StartChasing();

		GetWorld()->GetTimerManager().SetTimer(mChaserTimerHandle, this, &AGameLevelMode::chaserTick, 1.0f, true);
	}
	case GameState::PauseMenu: {
		/// stop timers
		/// display pauseMenu
		status = true;
		break;
	}
	case GameState::ScoreBoard: {
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
				setGameState(GameState::Countdown);
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

	int boardHeight(12);
	int boardWidth(12);

	// set the size of the game board
	if (!gameBoard->init(boardHeight, boardWidth)) {
		UE_LOG(LogTemp, Error, TEXT("Couldn't initialize game board"));
	};

	BoardLayout newLayout;
	gameBoard->spawnBoard(newLayout);

	FVector CameraLocation(boardWidth * 12.5f, boardWidth * 7.5f, boardHeight * -15.0f);
	FRotator CameraRotation(0.0f, 270.0f, 0.0f); // Y, Z, X for some reason
	mCamera = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation);
	mCamera->SetOwner(this);

	auto camera = mCamera->GetCameraComponent();
	camera->ProjectionMode = ECameraProjectionMode::Orthographic;
	camera->SetOrthoWidth(boardWidth * 75);

	auto controller = GetWorld()->GetFirstPlayerController();
	if (controller && (mCamera->HasActiveCameraComponent())) {
		controller->SetViewTarget(mCamera);
	}
	else {
		return nullptr;
	}
	return gameBoard;
}

bool AGameLevelMode::startCountdown()
{
	bool status(false);

	if (IsValid(mCountdownWidgetClass)) {
		mCountdownWidget = CreateWidget<UCountdownTimerWidget>(GetWorld(), mCountdownWidgetClass);
		if (mCountdownWidget) {
			mCountdownWidget->AddToViewport(1);
			mCountdownWidget->startCountdown(5);
			status = true;
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Countdown widget not created"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("mCountdownWidgetClass is nullptr"));
	}
	return status;
}

void AGameLevelMode::chaserTick()
{
	auto status = mChaser.nextMove();
	if (status == ChaseStatus::Lost || status == ChaseStatus::Won) {
		GetWorld()->GetTimerManager().ClearTimer(mChaserTimerHandle);
		if (status == ChaseStatus::Won) {
			mSession.updateScore(mChaser.getPathLenght());
		}
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
	}
	mSpawnedBoard->changeColor(mChaser.getCurrPosition());
}

void AGameLevelMode::setGameState(GameState const gameState) {
	UE_LOG(LogTemp, Error, TEXT("Setting game state"));
	mCurrentState = gameState;

	if (!handleNewState()) {
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
	mCountdownWidgetClass = LoadClass<UCountdownTimerWidget>(this, TEXT("/Game/Menus/CountdownWidget.CountdownWidget_C"));
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

	setGameState(GameState::MainMenu);
}
