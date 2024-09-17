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
    bool status(false);

    switch (mCurrentState)
    {
    case GameState::MainMenu: {
        mPostGameWidget->SetVisibility(ESlateVisibility::Hidden);
        mCountdownWidget->SetVisibility(ESlateVisibility::Hidden);

        mSession.startSession();

        status = !!showMainMenu();
        mMainMenuWidget->SetVisibility(ESlateVisibility::Visible);
        break;
    }
    case GameState::Countdown: {
        // hide mainMenu
        mMainMenuWidget->SetVisibility(ESlateVisibility::Hidden);
        mPostGameWidget->SetVisibility(ESlateVisibility::Hidden);
        mCountdownWidget->SetVisibility(ESlateVisibility::Visible);

        // spawn gameboard
        mSpawnedBoard = spawnGameBoard();

        status = mSpawnedBoard && startCountdown();
        break;
    }
    case GameState::Chasing: {
        mChaser.init(mSpawnedBoard->getLayout());
        mChaser.StartChasing();

        GetWorld()->GetTimerManager().SetTimer(mChaserTimerHandle, this, &AGameLevelMode::chaserTick, mSession.getOverflowSpeed(), true);
        status = true;
        break;
    }
    case GameState::PostGameMenu: {
        mSpawnedBoard->Destroy();
        mCamera->Destroy();

        mPostGameWidget->SetVisibility(ESlateVisibility::Visible);
        mPostGameWidget->AddToViewport();
        mCountdownWidget->SetVisibility(ESlateVisibility::Hidden);

        status = true;
        break;
    }
    case GameState::ScoreBoard: {
        /// not implemented
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
        UE_LOG(LogTemp, Error, TEXT("mMainMenu is nullptr"));
    }

    return status;
}

AGameBoardActor* AGameLevelMode::spawnGameBoard()
{
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
    if (!gameBoard->isInitialized())
    {
        if (!gameBoard->init(boardHeight, boardWidth)) {
            UE_LOG(LogTemp, Error, TEXT("Couldn't initialize game board"));
        };
    }

    BoardLayout newLayout;
    gameBoard->spawnBoard(newLayout);

    FVector CameraLocation(boardWidth * 15.0f, boardWidth * 7.5f, boardHeight * -15.0f);
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

    if (mCountdownWidget) {
        mCountdownWidget->AddToViewport(1);
        mCountdownWidget->startCountdown(mSession.getCountdownAmount());
        status = true;
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Countdown widget not created"));
    }
    return status;
}

void AGameLevelMode::chaserTick()
{
    auto status = mChaser.nextMove();
    mSpawnedBoard->changeColor(mChaser.getCurrPosition());
    mSession.updateScore(mChaser.getPathLenght());
    if (status == ChaseStatus::Lost || status == ChaseStatus::Won) {
        mPostGameWidget->onEnd(status, mSession.getScore());
        GetWorld()->GetTimerManager().ClearTimer(mChaserTimerHandle);
        if (status == ChaseStatus::Won) {
            mSession.onWin();
        }
        else {
            mSession.startSession();
        }
        setGameState(GameState::PostGameMenu);
    }
}

void AGameLevelMode::setGameState(GameState const gameState) {
    UE_LOG(LogTemp, Error, TEXT("Setting game state"));
    mCurrentState = gameState;

    if (!handleNewState()) {
        GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
        
        return;
    }
}

bool AGameLevelMode::initAllObjects() {
    if (mInitilized) {
        
        return false;
    }

    mMainMenuWidgetClass = LoadClass<UMainMenuWidget>(this, TEXT("/Game/Menus/MainMenuWidgetBP.MainMenuWidgetBP_C"));
    mCountdownWidgetClass = LoadClass<UCountdownTimerWidget>(this, TEXT("/Game/Menus/CountdownWidget.CountdownWidget_C"));
    mPostGameWidgetClass = LoadClass<UPostGameWidget>(this, TEXT("/Game/Menus/EndGameMenu.EndGameMenu_C"));
    if (IsValid(mMainMenuWidgetClass)) {
        mMainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), mMainMenuWidgetClass);
    }
    if (IsValid(mCountdownWidgetClass)) {
        mCountdownWidget = CreateWidget<UCountdownTimerWidget>(GetWorld(), mCountdownWidgetClass);
    }
    if (IsValid(mPostGameWidgetClass)) {
        mPostGameWidget = CreateWidget<UPostGameWidget>(GetWorld(), mPostGameWidgetClass);
    }

    mSpawnedBoard = NewObject<AGameBoardActor>(this);
    mInitilized = (mMainMenuWidgetClass && mCountdownWidgetClass && mPostGameWidgetClass && mSpawnedBoard);
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
